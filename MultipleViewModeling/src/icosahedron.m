function [vertices] = icosahedron(iter)
    t = (1 + sqrt(5)) / 2;
    a = sqrt(t) / ((5) ^ (1 / 4));
    b = 1 / ((sqrt(t) * (5 ^ (1 / 4))));
    c = a + 2 * b;
    d = a + b;
    
    vertices = [
        0,  a,  b;
        0, -a,  b;
        0,  a, -b;
        0, -a, -b;
        b,  0,  a;
       -b,  0,  a;
        b,  0, -a;
       -b,  0, -a;
        a,  b,  0;
        a, -b,  0;
       -a,  b,  0;
       -a, -b,  0];
    
    face_midpoints = (1/3) .* [
        d,  d,  d;
        d,  d, -d;
        d, -d,  d;
        d, -d, -d;
       -d,  d,  d;
       -d,  d, -d;
       -d, -d,  d;
       -d, -d, -d;
        0,  a,  c;
        0, -a,  c;
        0,  a, -c;
        0, -a, -c;
        c,  0,  a;
       -c,  0,  a;
        c,  0, -a;
       -c,  0, -a;
        a,  c,  0;
        a, -c,  0;
       -a,  c,  0;
       -a, -c,  0];
   
   % recursivelly update
   for i = 1 : iter,
       face_size = size(face_midpoints, 1);
       new_face_midpoints = rand(0, 3);
       new_vertices = rand(0,3);
       for k = 1 : face_size,
           
           % find the three vectices for the current face
           dt = (vertices(:, 1) - face_midpoints(k, 1)) .^ 2 + ...
                (vertices(:, 2) - face_midpoints(k, 2)) .^ 2 + ...
                (vertices(:, 3) - face_midpoints(k, 3)) .^ 2;
           [~, order] = sort(dt);
           face_vertices = vertices(order(1:3), :);
           
           % compute the new vertices
           v1 = (face_vertices(2, :) - face_vertices(1, :)) ./ 2 + ... 
                 face_vertices(1, :);
           v2 = (face_vertices(3, :) - face_vertices(1, :)) ./ 2 + ... 
                 face_vertices(1, :);
           v3 = (face_vertices(3, :) - face_vertices(2, :)) ./ 2 + ... 
                 face_vertices(2, :);
           v1 = v1 ./ norm(v1);
           v2 = v2 ./ norm(v2);
           v3 = v3 ./ norm(v3);
           new_vertices = unique([new_vertices; v1; v2; v3], 'rows');
           
           % compute new surface midpoints
           f_midpoint1 = (face_vertices(1, :) + v1 + v2) ./ 3;
           f_midpoint2 = (face_vertices(2, :) + v1 + v3) ./ 3;
           f_midpoint3 = (face_vertices(3, :) + v2 + v3) ./ 3;
           f_midpoint4 = (v1 + v2 + v3) ./ 3;
           new_face_midpoints = unique([new_face_midpoints; f_midpoint1; ...
               f_midpoint2; f_midpoint3; f_midpoint4], 'rows');
       end
       
       % update the new face and vertices
       vertices = unique([vertices; new_vertices], 'rows');
       face_midpoints = new_face_midpoints;
   end

end
