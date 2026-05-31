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

pub struct Duplicate_Field_Name_In_Struct_73;

impl Mutator for Duplicate_Field_Name_In_Struct_73 {
    fn name(&self) -> &str {
        "Duplicate_Field_Name_In_Struct_73"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut all_types = HashSet::new();
        let mut visitor = TypeCollector { types: &mut all_types };
        visitor.visit_file(file);
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.fields.len() > 0 {
                    let fields = &mut item_struct.fields;
                    let field_names = fields.iter().map(|f| f.ident.clone()).collect::<Vec<_>>();
                    let chosen_name = field_names.choose(&mut rng).cloned().flatten();
                    if let Some(chosen_name) = chosen_name {
                        let candidate_types = fields.iter().map(|f| f.ty.clone()).collect::<Vec<_>>();
                        let chosen_type = if candidate_types.len() > 1 {
                            candidate_types.choose(&mut rng).cloned()
                        } else {
                            None
                        };
                        let new_type = match chosen_type {
                            Some(ty) => ty,
                            None => {
                                let primitive_types = vec![
                                    parse_quote!(i32),
                                    parse_quote!(bool),
                                    parse_quote!(u8),
                                    parse_quote!(f64),
                                    parse_quote!(String),
                                ];
                                let available_primitives = primitive_types.iter()
                                    .filter(|ty| all_types.contains(&ty))
                                    .collect::<Vec<_>>();
                                if available_primitives.len() > 0 {
                                    available_primitives.choose(&mut rng).cloned().unwrap().clone()
                                } else {
                                    parse_quote!(u8)
                                }
                            }
                        };
                        let new_field = syn::Field {
                            attrs: vec![],
                            ident: Some(chosen_name),
                            ty: new_type,
                            colon_token: Some(token::Colon::default()),
                            vis: syn::Visibility::Inherited,
                            mutability: syn::FieldMutability::None,
                        };
                        let insert_pos = rng.gen_range(0..fields.len() + 1);
                        let mut fields_vec = fields.iter().cloned().collect::<Vec<_>>();
                        fields_vec.insert(insert_pos, new_field);
                        *fields = syn::Fields::Named(Punctuated::from_iter(fields_vec));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator duplicates a field name within a struct definition. It selects a struct with at least one field, picks an existing field name, and inserts a new field with the same name at a random position. The type for the duplicate field is chosen from: 1) another existing field's2) a primitive type already used elsewhere in the program; 3) default u8. This creates duplicate field names, causing compilation errors and testing the compiler's handling of duplicate identifiers in struct definitions, type resolution conflicts, and struct instantiation errors."
    }
}

struct TypeCollector<'a> {
    types: &'a mut HashSet<syn::Type>,
}

impl<'a> Visit<'a> for TypeCollector<'a> {
    fn visit_type(&mut self, node: &'a syn::Type) {
        self.types.insert(node.clone());
        syn::visit::visit_type(self, node);
    }
}