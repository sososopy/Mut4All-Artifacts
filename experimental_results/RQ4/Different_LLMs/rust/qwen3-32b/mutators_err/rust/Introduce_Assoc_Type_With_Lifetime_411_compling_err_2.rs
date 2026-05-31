use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeDef, Local, Pat, PatType, Path as SynPath,
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

pub struct Introduce_Assoc_Type_With_Lifetime_411;

impl Mutator for Introduce_Assoc_Type_With_Lifetime_411 {
    fn name(&self) -> &str {
        "Introduce_Assoc_Type_With_Lifetime_411"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_structs = HashSet::new();
        
        // Process structs first
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                let has_lifetime = s.generics.params.iter().any(|p| matches!(p, GenericParam::LifetimeDef(_)));
                if !has_lifetime {
                    // Add lifetime parameter 's
                    let lifetime = Lifetime::new("'s", Span::call_site());
                    s.generics.params.push(GenericParam::Lifetime(LifetimeDef {
                        attrs: vec![],
                        lifetime: lifetime.clone(),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    
                    // Add PhantomData field
                    if let Some(data) = s.content.as_mut() {
                        if let syn::Fields::Named(fields) = &mut data.fields {
                            fields.named.push(parse_quote! {
                                _phantom: std::marker::PhantomData<&'s ()>
                            });
                        }
                    }
                    modified_structs.insert(s.ident.clone());
                }
            }
        }
        
        // Process impl blocks for modified structs
        for item in &mut file.items {
            if let syn::Item::Impl(i) = item {
                if let Type::Path(ty_path) = &*i.self_ty {
                    if let Some(ident) = ty_path.path.get_ident() {
                        if modified_structs.contains(ident) {
                            // Add associated type
                            i.items.push(parse_quote! {
                                type Assoc<'a> = DocCursorImpl<'a>;
                            });
                            
                            // Modify first method's return type
                            if let Some(method) = i.items.iter_mut().find_map(|item| {
                                if let syn::ImplItem::Fn(fn_item) = item {
                                    Some(fn_item)
                                } else {
                                    None
                                }
                            }) {
                                if let ReturnType::Type(_, ty) = &mut method.sig.output {
                                    *ty = Box::new(parse_quote! {
                                        Self::Assoc<'_>
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}