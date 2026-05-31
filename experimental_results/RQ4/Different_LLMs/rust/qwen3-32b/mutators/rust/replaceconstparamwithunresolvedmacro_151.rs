use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprMacro, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::{self, VisitMut},
    *,
};

use crate::mutator::Mutator;

pub struct ReplaceConstParamWithUnresolvedMacro_151;

impl Mutator for ReplaceConstParamWithUnresolvedMacro_151 {
    fn name(&self) -> &str {
        "ReplaceConstParamWithUnresolvedMacro_151"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct ReplaceVisitor;
        impl VisitMut for ReplaceVisitor {
            fn visit_generic_param_mut(&mut self, i: &mut GenericParam) {
                if let GenericParam::Const(c) = i {
                    *i = GenericParam::Type(syn::TypeParam {
                        attrs: vec![],
                        ident: c.ident.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                        eq_token: None,
                        default: Some(parse_quote! { unresolved!() }),
                    });
                }
                visit_mut::visit_generic_param_mut(self, i);
            }
        }
        let mut visitor = ReplaceVisitor;
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "Replaces const parameters with unresolved macro invocations to potentially trigger ICEs."
    }
}