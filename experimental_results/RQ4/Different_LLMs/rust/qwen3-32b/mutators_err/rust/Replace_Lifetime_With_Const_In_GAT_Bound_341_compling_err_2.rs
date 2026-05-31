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

pub struct Replace_Lifetime_With_Const_In_GAT_Bound_341;

impl Mutator for Replace_Lifetime_With_Const_In_GAT_Bound_341 {
    fn name(&self) -> &str {
        "Replace_Lifetime_With_Const_In_GAT_Bound_341"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(trait_obj) = &mut *pat_type.ty {
                            self.process_trait_object(trait_obj);
                        }
                    }
                }
                if let ReturnType::Type(_, ty) = &mut func.sig.output {
                    if let Type::TraitObject(trait_obj) = &mut **ty {
                        self.process_trait_object(trait_obj);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::TraitObject(trait_obj) = &mut *pat_type.ty {
                                    self.process_trait_object(trait_obj);
                                }
                            }
                        }
                        if let ReturnType::Type(_, ty) = &mut method.sig.output {
                            if let Type::TraitObject(trait_obj) = &mut **ty {
                                self.process_trait_object(trait_obj);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces lifetime parameters in generic associated type (GAT) bounds of `dyn Trait` types with const generic expressions. This creates type/lifetime confusion by substituting a const argument in a position expecting a lifetime, challenging the compiler's type system and trait resolution logic."
    }
}

impl Replace_Lifetime_With_Const_In_GAT_Bound_341 {
    fn process_trait_object(&self, trait_obj: &mut TypeTraitObject) {
        for bound in &mut trait_obj.bounds {
            if let TypeParamBound::Trait(trait_bound) = bound {
                for segment in &mut trait_bound.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::AssocType(assoc_type) = arg {
                                for segment in &mut assoc_type.path.segments {
                                    if let PathArguments::AngleBracketed(gen_args) = &mut segment.arguments {
                                        for gen_arg in &mut gen_args.args {
                                            if let GenericArgument::Lifetime(lt) = gen_arg {
                                                *gen_arg = GenericArgument::Const(Expr::Lit(ExprLit {
                                                    attrs: vec![],
                                                    lit: Lit::Int(LitInt::new("1", Span::call_site())),
                                                    pat: PatLit {
                                                        eq_token: token::Eq::default(),
                                                        lit: Lit::Int(LitInt::new("1", Span::call_site())),
                                                    },
                                                }));
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