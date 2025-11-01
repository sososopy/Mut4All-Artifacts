use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Alter_Trait_Bounds_445;

impl Mutator for Alter_Trait_Bounds_445 {
    fn name(&self) -> &str {
        "Alter_Trait_Bounds_445"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TraitBoundVisitor {
            extra_trait_added: bool,
        }

        impl VisitMut for TraitBoundVisitor {
            fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
                if !self.extra_trait_added {
                    let extra_trait: ImplItem = parse_quote! {
                        trait Extra {}
                    };
                    i.items.insert(0, extra_trait);
                    self.extra_trait_added = true;
                }
                syn::visit_mut::visit_item_impl_mut(self, i);
            }

            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                for param in &mut i.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        let extra_bound: TypeParamBound = parse_quote!(Extra);
                        type_param.bounds.push(extra_bound);
                    }
                }
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
        }

        let mut visitor = TraitBoundVisitor { extra_trait_added: false };
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with generic parameters having trait bounds and adds an additional trait, `Extra`, to these bounds. This tests the compiler's handling of additional and potentially conflicting trait requirements."
    }
}