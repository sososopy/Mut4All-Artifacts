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

pub struct Alter_DynStar_To_Ref_Dyn_332;

impl Mutator for Alter_DynStar_To_Ref_Dyn_332 {
    fn name(&self) -> &str {
        "Alter_DynStar_To_Ref_Dyn_332"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Path(type_path) = &*pat_type.ty {
                            if type_path
                                .path
                                .segments
                                .iter()
                                .any(|seg| seg.ident == "dyn_star")
                            {
                                pat_type.ty = Box::new(syn::Type::Reference(syn::TypeReference {
                                    and_token: Default::default(),
                                    lifetime: None,
                                    mutability: None,
                                    elem: Box::new(syn::Type::TraitObject(
                                        syn::TypeTraitObject {
                                            dyn_token: Some(Default::default()),
                                            bounds: {
                                                let mut bounds = Punctuated::new();
                                                bounds.push(syn::TypeParamBound::Trait(
                                                    syn::TraitBound {
                                                        paren_token: None,
                                                        modifier: syn::TraitBoundModifier::None,
                                                        lifetimes: None,
                                                        path: syn::Path::from(syn::Ident::new(
                                                            "Trait",
                                                            Span::call_site(),
                                                        )),
                                                    },
                                                ));
                                                bounds
                                            },
                                        },
                                    )),
                                }));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for input in &mut func.sig.inputs {
                            if let syn::FnArg::Typed(pat_type) = input {
                                if let syn::Type::Path(type_path) = &*pat_type.ty {
                                    if type_path
                                        .path
                                        .segments
                                        .iter()
                                        .any(|seg| seg.ident == "dyn_star")
                                    {
                                        pat_type.ty =
                                            Box::new(syn::Type::Reference(syn::TypeReference {
                                                and_token: Default::default(),
                                                lifetime: None,
                                                mutability: None,
                                                elem: Box::new(syn::Type::TraitObject(
                                                    syn::TypeTraitObject {
                                                        dyn_token: Some(Default::default()),
                                                        bounds: {
                                                            let mut bounds = Punctuated::new();
                                                            bounds.push(
                                                                syn::TypeParamBound::Trait(
                                                                    syn::TraitBound {
                                                                        paren_token: None,
                                                                        modifier:
                                                                            syn::TraitBoundModifier::None,
                                                                        lifetimes: None,
                                                                        path: syn::Path::from(
                                                                            syn::Ident::new(
                                                                                "Trait",
                                                                                Span::call_site(),
                                                                            ),
                                                                        ),
                                                                    },
                                                                ),
                                                            );
                                                            bounds
                                                        },
                                                    },
                                                )),
                                            }));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        struct CallVisitor {
            calls: Vec<ExprCall>,
        }
        impl<'ast> Visit<'ast> for CallVisitor {
            fn visit_expr_call(&mut self, node: &'ast ExprCall) {
                self.calls.push(node.clone());
                syn::visit::visit_expr_call(self, node);
            }
        }
        let mut visitor = CallVisitor { calls: vec![] };
        visitor.visit_file(file);
        for call in visitor.calls {
            if let Expr::Path(ExprPath { path, .. }) = &*call.func {
                if path.segments.iter().any(|seg| seg.ident == "bar") {
                    let mut new_args = Punctuated::new();
                    for arg in call.args {
                        new_args.push(Expr::Reference(syn::ExprReference {
                            attrs: vec![],
                            and_token: Default::default(),
                            raw: Default::default(),
                            mutability: None,
                            expr: Box::new(arg),
                        }));
                    }
                    let new_call = ExprCall {
                        attrs: call.attrs,
                        func: call.func,
                        paren_token: call.paren_token,
                        args: new_args,
                    };
                    *file = syn::parse_quote!(#new_call);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions and impl methods that use the `dyn_star` type for parameters, replacing it with a reference to a `dyn Trait` trait object. It also updates function calls to pass arguments by reference. This transformation leverages the experimental `dyn_star` feature to induce type system stress, testing the compiler's handling of trait object conversions and reference semantics in both function signatures and call sites."
    }
}