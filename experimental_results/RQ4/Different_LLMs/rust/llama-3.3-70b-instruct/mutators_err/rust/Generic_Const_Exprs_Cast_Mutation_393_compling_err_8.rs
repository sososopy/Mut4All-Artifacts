use syn::{parse_quote, visit_mut::VisitMut, Expr, ExprCast, File, Ident, Item, ItemImpl, ImplItem, Path, PathArguments, PathSegment, punctuated::Punctuated, Stmt, Token, Type, TypePath};
use proc_macro2::{Span};

struct Generic_Const_Exprs_Cast_Mutation_393;

impl Generic_Const_Exprs_Cast_Mutation_393 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Cast_Mutation_393"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Method(method) = impl_item {
                        for statement in &mut method.block.stmts {
                            if let Stmt::Expr(expr, _) = statement {
                                if let Expr::Cast(expr_cast) = expr {
                                    if let Expr::Path(expr_path) = &*expr_cast.expr {
                                        if let Some(segment) = expr_path.path.segments.last() {
                                            let new_ident = match segment.ident.to_string().as_str() {
                                                "f64" => "u128",
                                                "u128" => "f64",
                                                _ => &segment.ident.to_string(),
                                            };
                                            let new_type = Type::Path(TypePath {
                                                qself: None,
                                                path: Path {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(vec![PathSegment {
                                                        ident: Ident::new(new_ident, Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    }]),
                                                },
                                            });
                                            expr_cast.ty = Box::new(new_type);
                                        }
                                    }
                                }
                            }
                        }
                    } else if let ImplItem::Const(const_item) = impl_item {
                        if let Expr::Cast(expr_cast) = &const_item.expr {
                            if let Expr::Path(expr_path) = &*expr_cast.expr {
                                if let Some(segment) = expr_path.path.segments.last() {
                                    let new_ident = match segment.ident.to_string().as_str() {
                                        "f64" => "u128",
                                        "u128" => "f64",
                                        _ => &segment.ident.to_string(),
                                    };
                                    let new_type = Type::Path(TypePath {
                                        qself: None,
                                        path: Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new(new_ident, Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    });
                                    expr_cast.ty = Box::new(new_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets Rust code that uses generic const expressions, specifically focusing on constant casts within trait implementations or generic types. It introduces subtle errors by modifying the type of constants or changing the casting operations to trigger bugs in the Rust compiler's handling of generic const expressions."
    }
}