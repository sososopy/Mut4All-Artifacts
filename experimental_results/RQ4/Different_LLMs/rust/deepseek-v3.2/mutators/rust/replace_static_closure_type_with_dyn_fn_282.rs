use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Static_Closure_Type_With_Dyn_Fn_282;

impl Mutator for Replace_Static_Closure_Type_With_Dyn_Fn_282 {
    fn name(&self) -> &str {
        "Replace_Static_Closure_Type_With_Dyn_Fn_282"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let fn_traits = ["Fn", "FnMut", "FnOnce"];
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Type::Path(type_path) = &*item_static.ty {
                    let path_str = quote!(#type_path).to_string();
                    if path_str.contains("dyn") {
                        continue;
                    }
                }
                if let Expr::Closure(expr_closure) = &*item_static.expr {
                    let ret_ty = match &expr_closure.output {
                        ReturnType::Default => None,
                        ReturnType::Type(_, ty) => Some(ty.clone()),
                    };
                    let inferred_ty = if let Some(ret_ty) = ret_ty {
                        *ret_ty
                    } else {
                        if let Expr::Block(expr_block) = &*expr_closure.body {
                            if let Some(stmt) = expr_block.block.stmts.last() {
                                if let Stmt::Expr(expr, _) = stmt {
                                    let ty_str = quote!(#expr).to_string();
                                    if ty_str.ends_with("i32") {
                                        parse_quote!(i32)
                                    } else if ty_str.ends_with("usize") {
                                        parse_quote!(usize)
                                    } else if ty_str.ends_with("bool") {
                                        parse_quote!(bool)
                                    } else if ty_str.ends_with("String") {
                                        parse_quote!(String)
                                    } else {
                                        parse_quote!(())
                                    }
                                } else {
                                    parse_quote!(())
                                }
                            } else {
                                parse_quote!(())
                            }
                        } else {
                            parse_quote!(())
                        }
                    };
                    let chosen_trait = fn_traits.choose(&mut rng).unwrap_or(&"Fn");
                    let trait_bound = TypeParamBound::Trait(TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: None,
                        path: syn::Path {
                            leading_colon: None,
                            segments: {
                                let mut segments = Punctuated::new();
                                segments.push(PathSegment {
                                    ident: Ident::new(chosen_trait, Span::call_site()),
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: {
                                            let mut args = Punctuated::new();
                                            args.push(GenericArgument::Type(Type::Tuple(TypeTuple {
                                                paren_token: token::Paren::default(),
                                                elems: Punctuated::new(),
                                            })));
                                            args.push(GenericArgument::Type(inferred_ty));
                                            args
                                        },
                                        gt_token: token::Gt::default(),
                                    }),
                                });
                                segments
                            },
                        },
                    });
                    let mut bounds = Punctuated::new();
                    bounds.push(trait_bound);
                    let trait_object = Type::TraitObject(TypeTraitObject {
                        dyn_token: Some(token::Dyn::default()),
                        bounds,
                    });
                    item_static.ty = Box::new(trait_object);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static item declarations with explicit type annotations and closure initializers. It replaces the explicit type with a dynamically chosen `dyn Fn`, `dyn FnMut`, or `dyn FnOnce` trait object type, where the return type is inferred from the closure's body or explicit return annotation. This transformation introduces dynamic dispatch and trait object compatibility issues, testing the compiler's handling of closure-to-trait-object conversions, type inference edge cases, and static item initialization with dynamic types."
    }
}