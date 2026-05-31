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

pub struct Box_Pointer_To_Rc_In_Generator_Context_11;

impl Mutator for Box_Pointer_To_Rc_In_Generator_Context_11 {
    fn name(&self) -> &str {
        "Box_Pointer_To_Rc_In_Generator_Context_11"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut modified = false;
                for field in &mut item_struct.fields {
                    if let Type::Path(TypePath { path, .. }) = &mut field.ty {
                        if path.segments.len() == 1 && path.segments[0].ident == "Box" {
                            if let PathArguments::AngleBracketed(args) = &mut path.segments[0].arguments {
                                if let Some(GenericArgument::Type(Type::Path(TypePath { path: inner_path, .. }))) = args.args.first() {
                                    if inner_path.segments.iter().any(|seg| seg.ident == "Gen") {
                                        path.segments[0].ident = Ident::new("Rc", Span::call_site());
                                        modified = true;
                                    }
                                }
                            }
                        }
                    }
                }
                if modified {
                    file.items.insert(0, parse_quote! { use std::rc::Rc; });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets structures that use `Box` to hold generator-related data. It replaces `Box<T>` with `Rc<T>` if `T` is associated with a generator, requiring shared ownership semantics. This change tests the interaction between different pointer types and generator contexts, ensuring the codebase adapts to reference counting and shared ownership."
    }
}