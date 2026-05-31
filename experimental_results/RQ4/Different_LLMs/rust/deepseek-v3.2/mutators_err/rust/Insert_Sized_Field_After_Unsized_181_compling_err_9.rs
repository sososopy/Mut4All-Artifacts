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

pub struct Insert_Sized_Field_After_Unsized_181;

impl Mutator for Insert_Sized_Field_After_Unsized_181 {
    fn name(&self) -> &str {
        "Insert_Sized_Field_After_Unsized_181"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut sized_types = HashSet::new();
        let mut field_names = HashSet::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                for field in &item_struct.fields {
                    if let Some(ident) = &field.ident {
                        field_names.insert(ident.clone());
                    }
                    if let Type::Slice(_) = &field.ty {
                        continue;
                    }
                    if let Type::TraitObject(_) = &field.ty {
                        continue;
                    }
                    if let Type::Array(_) = &field.ty {
                        continue;
                    }
                    if let Type::Path(type_path) = &field.ty {
                        sized_types.insert(type_path.clone());
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            sized_types.insert(type_path.clone());
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        sized_types.insert(type_path.clone());
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut unsized_index = None;
                for (i, field) in item_struct.fields.iter().enumerate() {
                    if let Type::Slice(_) = &field.ty {
                        unsized_index = Some(i);
                        break;
                    }
                    if let Type::TraitObject(_) = &field.ty {
                        unsized_index = Some(i);
                        break;
                    }
                    if let Type::Array(_) = &field.ty {
                        unsized_index = Some(i);
                        break;
                    }
                }
                if let Some(index) = unsized_index {
                    let new_field_name = if let Some(existing_field) = item_struct.fields.iter().find(|f| f.ident.is_some()) {
                        let base_name = existing_field.ident.clone().unwrap();
                        let mut candidate = format!("{}_extra", base_name);
                        while field_names.contains(&Ident::new(&candidate, Span::call_site())) {
                            candidate = format!("{}_extra_{}", base_name, thread_rng().gen_range(0..100));
                        }
                        Ident::new(&candidate, Span::call_site())
                    } else {
                        Ident::new("extra_field", Span::call_site())
                    };
                    let new_field_type = if sized_types.is_empty() {
                        parse_quote!(u8)
                    } else {
                        sized_types.iter().next().unwrap().clone()
                    };
                    let new_field = syn::Field {
                        attrs: Vec::new(),
                        ident: Some(new_field_name),
                        ty: syn::Type::Path(new_field_type),
                        colon_token: Some(Default::default()),
                        mutability: syn::FieldMutability::None,
                        vis: syn::Visibility::Inherited,
                    };
                    if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                        fields_named.named.insert(index + 1, new_field);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions containing unsized fields (slices, trait objects, arrays). It identifies the first unsized field and inserts a new sized field immediately after it. The new field's type is chosen from existing sized types in the program, or defaults to u8 if none are available. The field name is derived from an existing field name with an '_extra' suffix to avoid conflicts. This transformation aims to trigger layout calculation errors in the compiler, particularly when a sized field follows an unsized field, potentially exposing internal compiler errors related to struct memory layout."
    }
}