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

pub struct Modify_Struct_Field_To_Tuple_66;

impl Mutator for Modify_Struct_Field_To_Tuple_66 {
    fn name(&self) -> &str {
        "Modify_Struct_Field_To_Tuple_66"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generic_param) = item_struct.generics.params.first() {
                    if let GenericParam::Type(type_param) = generic_param {
                        if let Some(field) = item_struct.fields.iter_mut().next() {
                            let field_type = &field.ty;
                            if let Type::Path(type_path) = field_type {
                                if type_path.path.segments.last().unwrap().ident == type_param.ident {
                                    field.ty = parse_quote!((#field_type, #field_type));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with generic parameters and modifies one of its fields to use a tuple of the generic parameter. This transformation increases the complexity of the struct's field type, testing the compiler's ability to handle changes in data layout and size, especially in generic contexts."
    }
}