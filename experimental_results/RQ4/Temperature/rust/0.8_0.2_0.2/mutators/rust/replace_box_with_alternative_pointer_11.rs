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

pub struct Replace_Box_With_Alternative_Pointer_11;

impl Mutator for Replace_Box_With_Alternative_Pointer_11 {
    fn name(&self) -> &str {
        "Replace_Box_With_Alternative_Pointer_11"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Path(TypePath { path, .. }) = &field.ty {
                        if path.segments.len() == 1 && path.segments[0].ident == "Box" {
                            let mut rng = thread_rng();
                            if rng.gen_bool(0.5) {
                                // Replace with raw pointer
                                field.ty = parse_quote!(*const G::Gen);
                            } else {
                                // Replace with reference
                                field.ty = parse_quote!(&'a G::Gen);
                                if !item_struct.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_))) {
                                    item_struct.generics.params.push(GenericParam::Lifetime(LifetimeParam::new(Lifetime::new("'a", Span::call_site()))));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct fields using `Box` for generator state management. It replaces `Box` with either a raw pointer or a reference, introducing variations in memory management strategies. This transformation tests the compiler's handling of different pointer types in generator contexts, potentially revealing issues with aliasing, lifetime management, or type resolution."
    }
}