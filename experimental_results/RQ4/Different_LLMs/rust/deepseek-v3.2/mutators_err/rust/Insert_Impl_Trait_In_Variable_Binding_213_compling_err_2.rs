use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStatic, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, PathSegment, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, TypeReference, parse_quote,
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

pub struct Insert_Impl_Trait_In_Variable_Binding_213;

impl Mutator for Insert_Impl_Trait_In_Variable_Binding_213 {
    fn name(&self) -> &str {
        "Insert_Impl_Trait_In_Variable_Binding_213"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ImplTraitVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets local variable declarations (let, const, static) with concrete type annotations. It analyzes the concrete type to find a suitable trait that the type implements (from prelude or in scope). The type annotation is then replaced with `impl Trait` (or `&impl Trait`/`&mut impl Trait` matching reference modifiers). This transformation tests the compiler's handling of `impl Trait` in binding positions, potentially triggering edge cases in opaque type metadata generation, HIR mapping, and type inference for variable bindings."
    }
}

struct ImplTraitVisitor;

impl VisitMut for ImplTraitVisitor {
    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Some((pat, ty)) = extract_pat_type_mut(local) {
            if let Some((base_type, is_ref, is_mut_ref)) = extract_base_type(&ty) {
                if let Some(trait_path) = find_suitable_trait_for_type(&base_type) {
                    let new_ty = construct_impl_trait_type(trait_path, is_ref, is_mut_ref);
                    *ty = Box::new(new_ty);
                }
            }
        }
        visit_mut::visit_local_mut(self, local);
    }

    fn visit_item_const_mut(&mut self, item: &mut ItemConst) {
        if let Type::Path(type_path) = &*item.ty {
            if let Some(trait_path) = find_suitable_trait_for_type(&type_path.path) {
                item.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                    impl_token: token::Impl { span: Span::call_site() },
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: trait_path,
                        }));
                        bounds
                    },
                }));
            }
        }
        visit_mut::visit_item_const_mut(self, item);
    }

    fn visit_item_static_mut(&mut self, item: &mut ItemStatic) {
        if let Type::Path(type_path) = &*item.ty {
            if let Some(trait_path) = find_suitable_trait_for_type(&type_path.path) {
                item.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                    impl_token: token::Impl { span: Span::call_site() },
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: trait_path,
                        }));
                        bounds
                    },
                }));
            }
        }
        visit_mut::visit_item_static_mut(self, item);
    }
}

fn extract_pat_type_mut(local: &mut Local) -> Option<(&mut Pat, &mut Box<Type>)> {
    if let Pat::Type(pat_type) = &mut local.pat {
        Some((&mut pat_type.pat, &mut pat_type.ty))
    } else {
        None
    }
}

fn extract_base_type(ty: &Type) -> Option<(SynPath, bool, bool)> {
    match ty {
        Type::Reference(type_ref) => {
            let is_mut = type_ref.mutability.is_some();
            if let Type::Path(type_path) = &*type_ref.elem {
                Some((type_path.path.clone(), true, is_mut))
            } else {
                None
            }
        }
        Type::Path(type_path) => Some((type_path.path.clone(), false, false)),
        _ => None,
    }
}

fn find_suitable_trait_for_type(type_path: &SynPath) -> Option<SynPath> {
    let type_name = type_path.segments.last()?.ident.to_string();
    
    let candidate_traits = match type_name.as_str() {
        "i8" | "i16" | "i32" | "i64" | "i128" | "isize" |
        "u8" | "u16" | "u32" | "u64" | "u128" | "usize" |
        "f32" | "f64" | "bool" | "char" => vec!["Copy", "Clone", "Debug", "Sized"],
        "String" | "Vec" | "Option" | "Result" => vec!["Clone", "Debug", "Sized"],
        "Box" | "Rc" | "Arc" => vec!["Clone", "Debug", "Deref"],
        "RefCell" | "Mutex" | "RwLock" => vec!["Debug"],
        "HashMap" | "BTreeMap" | "HashSet" | "BTreeSet" => vec!["Debug", "Clone"],
        _ => vec!["Debug", "Clone", "Sized"],
    };
    
    for trait_name in candidate_traits {
        let mut segments = Punctuated::new();
        segments.push(PathSegment {
            ident: Ident::new(trait_name, Span::call_site()),
            arguments: PathArguments::None,
        });
        let trait_path = SynPath {
            leading_colon: None,
            segments,
        };
        return Some(trait_path);
    }
    
    None
}

fn construct_impl_trait_type(trait_path: SynPath, is_ref: bool, is_mut_ref: bool) -> Type {
    let impl_trait = Type::ImplTrait(TypeImplTrait {
        impl_token: token::Impl { span: Span::call_site() },
        bounds: {
            let mut bounds = Punctuated::new();
            bounds.push(TypeParamBound::Trait(TraitBound {
                paren_token: None,
                modifier: TraitBoundModifier::None,
                lifetimes: None,
                path: trait_path,
            }));
            bounds
        },
    });
    
    if is_ref {
        Type::Reference(TypeReference {
            and_token: token::And { spans: [Span::call_site()] },
            lifetime: None,
            mutability: if is_mut_ref {
                Some(token::Mut { span: Span::call_site() })
            } else {
                None
            },
            elem: Box::new(impl_trait),
        })
    } else {
        impl_trait
    }
}