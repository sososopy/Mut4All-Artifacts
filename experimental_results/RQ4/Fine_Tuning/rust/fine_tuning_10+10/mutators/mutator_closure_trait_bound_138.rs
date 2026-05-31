use syn::{File, Item, Stmt, Expr, Type, ExprCast, TypePath, PathSegment, PathArguments, token::Semi};
use syn::punctuated::Punctuated;
use proc_macro2::{Ident, Span};
use rand::thread_rng;
use crate::mutator::Mutator;

pub struct Mutator_Closure_Trait_Bound_138;

impl Mutator for Mutator_Closure_Trait_Bound_138 {
    fn name(&self) -> &str {
        "Mutator_Closure_Trait_Bound_138"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(Expr::Closure(expr_closure), _) = stmt {
                        if let Expr::Cast(expr_cast) = &*expr_closure.body {
                            if let Type::Reference(type_ref) = &*expr_cast.ty {
                                let mut rng = thread_rng();
                                let new_type = Ident::new("SomeOtherType", Span::call_site());
                                let new_cast = ExprCast {
                                    attrs: expr_cast.attrs.clone(),
                                    expr: expr_cast.expr.clone(),
                                    as_token: expr_cast.as_token,
                                    ty: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: new_type,
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })),
                                };
                                *stmt = Stmt::Expr(Expr::Cast(new_cast), None);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets closures within functions that perform type casting or dereferencing operations. It identifies type conversions within these closures and modifies the target type to an incompatible or non-const trait type. This is done by changing the target type in the cast to a type that does not satisfy the required trait bounds, using an existing type in the seed program to maximize the chance of triggering a compiler bug."
    }
}