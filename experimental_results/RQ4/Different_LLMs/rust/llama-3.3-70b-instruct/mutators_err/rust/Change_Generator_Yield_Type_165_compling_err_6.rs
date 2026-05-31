use syn::{parse_quote, ReturnType, Type, TypePath, Stmt, Expr};
use crate::mutator::Mutator;

struct Change_Generator_Yield_Type_165;

impl Mutator for Change_Generator_Yield_Type_165 {
    fn name(&self) -> &str {
        "Change_Generator_Yield_Type_165"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: syn::Path {
                            leading_colon: None,
                            segments,
                        },
                    }) = &**ty
                    {
                        if segments.len() == 1 && segments[0].ident == "Generator" {
                            let new_type = parse_quote! { String };
                            *ty = Box::new(new_type);
                        }
                    }
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Yield(yield_expr) = expr {
                            if let Some(path) = yield_expr.expr.as_ref() {
                                if let Expr::Path(_) = path {
                                    yield_expr.expr = Some(Box::new(parse_quote! { (#path).to_string() }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generator functions and changes their yield type to String. It also modifies the yielded values to be converted to String using the to_string method. This transformation tests the compiler's handling of generator yield types and type conversions."
    }
}