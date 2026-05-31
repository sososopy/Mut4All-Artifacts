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

pub struct Replace_Explicit_Bound_With_Impl_Trait_222;

impl Mutator for Replace_Explicit_Bound_With_Impl_Trait_222 {
    fn name(&self) -> &str {
        "Replace_Explicit_Bound_With_Impl_Trait_222"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let mut new_inputs = Punctuated::new();
                        for input in method.sig.inputs.iter_mut() {
                            if let FnArg::Typed(PatType { ty, pat, .. }) = input {
                                if let Type::Path(type_path) = &**ty {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        let ident_str = segment.ident.to_string();
                                        if ident_str == "str" {
                                            let impl_trait = Type::ImplTrait(TypeImplTrait {
                                                impl_token: token::Impl { span: Span::call_site() },
                                                bounds: {
                                                    let mut bounds = Punctuated::new();
                                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                                        paren_token: None,
                                                        modifier: TraitBoundModifier::None,
                                                        lifetimes: None,
                                                        path: syn::Path {
                                                            leading_colon: None,
                                                            segments: {
                                                                let mut segs = Punctuated::new();
                                                                segs.push(PathSegment {
                                                                    ident: Ident::new("AsRef", Span::call_site()),
                                                                    arguments: PathArguments::AngleBracketed(
                                                                        AngleBracketedGenericArguments {
                                                                            colon2_token: None,
                                                                            lt_token: token::Lt::default(),
                                                                            args: {
                                                                                let mut args = Punctuated::new();
                                                                                args.push(GenericArgument::Type(
                                                                                    Type::Path(TypePath {
                                                                                        qself: None,
                                                                                        path: syn::Path::from(Ident::new("str", Span::call_site())),
                                                                                    })
                                                                                ));
                                                                                args
                                                                            },
                                                                            gt_token: token::Gt::default(),
                                                                        }
                                                                    ),
                                                                });
                                                                segs
                                                            },
                                                        },
                                                    }));
                                                    bounds
                                                },
                                            });
                                            *ty = Box::new(impl_trait);
                                        }
                                    }
                                }
                            }
                            new_inputs.push(input.clone());
                        }
                        method.sig.inputs = new_inputs;
                        method.block = parse_quote! { {} };
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations, replacing explicit type parameters with impl Trait syntax. Specifically, it changes &str parameters to impl AsRef<str> in method signatures. This creates a mismatch between the trait definition and implementation, introducing implicit generic parameters. The function body is replaced with an empty block to cause type errors. This transformation aims to trigger compiler ICEs during MIR optimization passes like polymorphization or inlining due to mismatched type parameter counts."
    }
}