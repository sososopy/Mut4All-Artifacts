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

pub struct Alter_Type_Aliases_In_Structs_264;

impl Mutator for Alter_Type_Aliases_In_Structs_264 {
    fn name(&self) -> &str {
        "Alter_Type_Aliases_In_Structs_264"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut alias_map = std::collections::HashMap::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                alias_map.insert(item_type.ident.to_string(), item_type.ty.clone());
            }
        }
        struct TypeAliasExpander<'a> {
            alias_map: &'a std::collections::HashMap<String, Box<Type>>,
        }
        impl<'a> VisitMut for TypeAliasExpander<'a> {
            fn visit_type_mut(&mut self, node: &mut Type) {
                if let Type::Path(type_path) = node {
                    if type_path.qself.is_none() && type_path.path.segments.len() == 1 {
                        let ident = &type_path.path.segments[0].ident;
                        if let Some(expanded_type) = self.alias_map.get(&ident.to_string()) {
                            *node = (**expanded_type).clone();
                            return;
                        }
                    }
                }
                syn::visit_mut::visit_type_mut(self, node);
            }
        }
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    let mut expander = TypeAliasExpander { alias_map: &alias_map };
                    expander.visit_type_mut(&mut field.ty);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs that use type aliases for their fields. It replaces these aliases with their expanded type definitions, effectively inlining the alias. This transformation can reveal how the compiler handles type alias resolution within structs, especially in the context of generics and trait bounds, and may expose bugs related to type expansion and name resolution."
    }
}