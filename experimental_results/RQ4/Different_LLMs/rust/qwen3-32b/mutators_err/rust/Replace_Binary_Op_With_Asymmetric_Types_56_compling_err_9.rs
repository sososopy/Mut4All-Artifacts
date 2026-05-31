use syn::visit_mut::visit_expr_mut;

pub struct Replace_Binary_Op_With_Asymmetric_Types_56;

impl Mutator for Replace_Binary_Op_With_Asymmetric_Types_56 {
    fn name(&self) -> &str {
        "Replace_Binary_Op_With_Asymmetric_Types_56"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut variables = HashMap::new();
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = *stmt {
                        if let Pat::Type(pat_type) = &local.pat {
                            if let Pat::Ident(ident_pat) = &*pat_type.pat {
                                if let Some(ty) = pat_type.ty {
                                    if is_numeric_type(&*ty) {
                                        variables.insert(ident_pat.ident.to_string(), ty);
                                    }
                                }
                            }
                        }
                    }
                }

                let mut visitor = BinaryOpVisitor {
                    variables: variables,
                };
                visitor.visit_block_mut(&mut func.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces binary operations between variables of the same numeric type with operations between variables of different numeric types in the same scope. This transformation introduces type mismatches in arithmetic operations, potentially exposing compiler bugs in const evaluation, type coercion, or unsafe arithmetic checks."
    }
}

struct BinaryOpVisitor {
    variables: HashMap<String, Box<Type>>,
}

impl VisitMut for BinaryOpVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Binary(expr_binary) = expr {
            if let Expr::Path(left_path) = &*expr_binary.left {
                if let Some(left_ident) = left_path.path.get_ident() {
                    if let Some(left_type) = self.variables.get(&left_ident.to_string()) {
                        if is_numeric_type(&**left_type) {
                            if let Expr::Path(right_path) = &*expr_binary.right {
                                if let Some(right_ident) = right_path.path.get_ident() {
                                    if let Some(right_type) = self.variables.get(&right_ident.to_string()) {
                                        if is_same_type(&**left_type, &**right_type) {
                                            if let Some(other_name) = find_other_variable(&self.variables, &**left_type) {
                                                let new_right = parse_quote!(#other_name);
                                                *expr_binary.right = new_right;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        visit_expr_mut(self, expr);
    }
}

fn is_numeric_type(ty: &Type) -> bool {
    if let Type::Path(type_path) = ty {
        if let Some(segment) = type_path.path.get_ident() {
            let ident = segment.to_string();
            ["i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64", "u128", "f32", "f64"]
                .contains(&ident.as_str())
        } else {
            false
        }
    } else {
        false
    }
}

fn is_same_type(a: &Type, b: &Type) -> bool {
    match (a, b) {
        (Type::Path(a_path), Type::Path(b_path)) => {
            if a_path.qself.is_some() || b_path.qself.is_some() {
                return false;
            }
            let a_segments = &a_path.path.segments;
            let b_segments = &b_path.path.segments;
            if a_segments.len() != 1 || b_segments.len() != 1 {
                return false;
            }
            let a_ident = a_segments.first().unwrap().ident.to_string();
            let b_ident = b_segments.first().unwrap().ident.to_string();
            a_ident == b_ident
        },
        _ => false,
    }
}

fn find_other_variable<'a>(
    variables: &'a HashMap<String, Box<Type>>,
    current_type: &'a Type,
) -> Option<&'a String> {
    for (name, ty) in variables {
        if !is_same_type(&**ty, current_type) {
            return Some(name);
        }
    }
    None
}