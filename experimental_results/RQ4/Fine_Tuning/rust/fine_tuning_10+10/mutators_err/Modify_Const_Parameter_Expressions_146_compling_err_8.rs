use crate::mutator::Mutator;
use syn::parse_quote;
use syn::Item;
use syn::ReturnType;
use syn::Type;
use syn::TypePath;
use syn::PathArguments;
use syn::GenericArgument;
use syn::Expr;
use syn::Stmt;

pub struct Modify_Const_Parameter_Expressions_146;

impl Mutator for Modify_Const_Parameter_Expressions_146 {
    fn name(&self) -> &str {
        "Modify_Const_Parameter_Expressions_146"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Path(TypePath { path, .. }) = &**return_type {
                        for segment in &mut path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Const(expr) = arg {
                                        if let Expr::Block(expr_block) = expr {
                                            if let Some(stmt) = expr_block.block.stmts.first() {
                                                if let Stmt::Expr(Expr::Binary(expr_binary), _) = stmt {
                                                    let new_expr: Expr = parse_quote! {
                                                        { #expr_binary + #expr_binary }
                                                    };
                                                    *arg = GenericArgument::Const(new_expr);
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
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with generic const parameters returning a struct using const expressions. It modifies the const expression in the return type to include a new operation or change an existing one, creating a more complex expression that combines the existing const parameters in a non-trivial way."
    }
}