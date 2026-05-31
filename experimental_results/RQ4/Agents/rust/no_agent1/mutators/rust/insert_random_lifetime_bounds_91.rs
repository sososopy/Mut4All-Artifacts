use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, thread_rng};
use std::collections::HashSet;
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident as SynIdent,
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
};

use crate::mutator::Mutator;

pub struct Insert_Random_Lifetime_Bounds_91;

impl Mutator for Insert_Random_Lifetime_Bounds_91 {
    fn name(&self) -> &str {
        "Insert_Random_Lifetime_Bounds_91"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                let lifetime_name = format!("l{}", rng.gen_range(0..100));
                let lifetime_ident = Ident::new(&lifetime_name, Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: vec![],
                    lifetime: Lifetime {
                        apostrophe: Span::call_site(),
                        ident: lifetime_ident.clone(),
                    },
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                
                let bound_lifetime = Lifetime {
                    apostrophe: Span::call_site(),
                    ident: lifetime_ident,
                };
                
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &mut *pat_type.ty {
                            type_ref.lifetime = Some(bound_lifetime.clone());
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces random lifetime parameters and applies them to reference types in function signatures. By adding arbitrary lifetimes, it aims to stress the compiler's lifetime resolution and elision mechanisms, potentially uncovering bugs related to lifetime inference and borrow checking."
    }
}