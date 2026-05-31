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

pub struct Modify_Constant_Parameters_In_Structs_144;

impl Mutator for Modify_Constant_Parameters_In_Structs_144 {
    fn name(&self) -> &str {
        "Modify_Constant_Parameters_In_Structs_144"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_static_lifetime = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Reference(type_ref) = &const_param.ty {
                            if type_ref.lifetime
                                == Some(Lifetime::new("'static", Span::call_site()))
                            {
                                has_static_lifetime = true;
                                break;
                            }
                        }
                    }
                }
                if has_static_lifetime {
                    let mut new_params = Punctuated::new();
                    new_params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new("'a", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    for param in &item_struct.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Reference(type_ref) = &const_param.ty {
                                if type_ref.lifetime
                                    == Some(Lifetime::new("'static", Span::call_site()))
                                {
                                    continue;
                                }
                            }
                        }
                        new_params.push(param.clone());
                    }
                    item_struct.generics.params = new_params;
                    if let Fields::Named(fields_named) = &mut item_struct.fields {
                        for field in &mut fields_named.named {
                            if let Type::Reference(type_ref) = &mut field.ty {
                                if type_ref.lifetime
                                    == Some(Lifetime::new("'static", Span::call_site()))
                                {
                                    type_ref.lifetime = Some(Lifetime::new("'a", Span::call_site()));
                                }
                            }
                        }
                    }
                    if let Fields::Unnamed(fields_unnamed) = &mut item_struct.fields {
                        for field in &mut fields_unnamed.unnamed {
                            if let Type::Reference(type_ref) = &mut field.ty {
                                if type_ref.lifetime
                                    == Some(Lifetime::new("'static", Span::call_site()))
                                {
                                    type_ref.lifetime = Some(Lifetime::new("'a", Span::call_site()));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with constant parameters that use a 'static lifetime reference. It replaces the 'static lifetime with a generic lifetime parameter 'a and updates the struct's fields accordingly. This transformation tests the compiler's ability to handle lifetime parameterization in constant generic contexts, potentially revealing issues with lifetime resolution and type checking."
    }
}