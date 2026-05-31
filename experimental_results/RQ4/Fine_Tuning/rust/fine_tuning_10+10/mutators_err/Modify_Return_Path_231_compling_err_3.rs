use crate::mutator::Mutator;

pub struct Modify_Return_Path_231;

impl Mutator for Modify_Return_Path_231 {
    fn name(&self) -> &str {
        "Modify_Return_Path_231"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ref return_type) = func.sig.output {
                    if let Type::ImplTrait(_) = **return_type {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(Expr::If(if_expr), _) = stmt {
                                if let Expr::Block(ref mut else_block) = *if_expr.else_branch.as_mut().unwrap().1 {
                                    if let Some(last_expr) = else_block.block.stmts.last_mut() {
                                        if let Stmt::Expr(expr, _) = last_expr {
                                            *expr = parse_quote! { Box::new(#expr) };
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with conditional logic to modify the return path. Specifically, it changes the return type in the else branch to a boxed dynamic trait object, potentially causing type inconsistencies. This manipulation explores the inner workings of Rust's type inference and can reveal compiler bugs related to type resolution and trait implementation."
    }
}