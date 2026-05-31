use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Array_Size_With_Generic_Associated_Const_340;

impl Mutator for Replace_Array_Size_With_Generic_Associated_Const_340 {
    fn name(&self) -> &str {
        "Replace_Array_Size_With_Generic_Associated_Const_340"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_needed = false;
        let mut visitor = ArraySizeVisitor {
            modified: false,
            trait_needed: &mut trait_needed,
        };
        visitor.visit_file_mut(file);
        
        if trait_needed {
            let trait_item: Item = parse_quote! {
                trait __MutatorTrait {
                    const ASSOC: usize = const { std::mem::size_of::<Self>() };
                }
            };
            file.items.insert(0, trait_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array type definitions within structs, enums, and unions that use constant sizes. It replaces the array size with a generic const expression that depends on a trait's associated constant, which itself uses a generic const expression like `size_of::<T>()`. If the enclosing item lacks generic parameters, one is added. The trait is defined if not present. This transformation creates complex generic const evaluation scenarios that stress the compiler's const evaluation and trait resolution systems, potentially triggering incomplete feature handling or ICEs."
    }
}

struct ArraySizeVisitor<'a> {
    modified: bool,
    trait_needed: &'a mut bool,
}

impl<'a> VisitMut for ArraySizeVisitor<'a> {
    fn visit_item_mut(&mut self, item: &mut Item) {
        match item {
            Item::Struct(s) => self.process_generic_item(s),
            Item::Enum(e) => self.process_generic_item(e),
            Item::Union(u) => self.process_generic_item(u),
            _ => {}
        }
        visit_mut::visit_item_mut(self, item);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(arr) = ty {
            if let Expr::Lit(expr_lit) = &*arr.len {
                if let syn::Lit::Int(_) = &expr_lit.lit {
                    *self.trait_needed = true;
                    self.modified = true;
                    
                    let new_len: Expr = parse_quote! {
                        const { __MutatorTrait::<T>::ASSOC }
                    };
                    arr.len = Box::new(new_len);
                }
            }
        }
        visit_mut::visit_type_mut(self, ty);
    }
}

impl<'a> ArraySizeVisitor<'a> {
    fn process_generic_item<T: GenericItem>(&mut self, item: &mut T) {
        let has_generic_param = !item.generics().params.is_empty();
        if !has_generic_param {
            item.generics_mut().params.push(parse_quote!(T));
        }
        
        let mut has_trait_bound = false;
        for param in &item.generics().params {
            if let GenericParam::Type(type_param) = param {
                for bound in &type_param.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if trait_bound.path.is_ident("__MutatorTrait") {
                            has_trait_bound = true;
                        }
                    }
                }
            }
        }
        
        if !has_trait_bound {
            let where_clause = item.generics_mut().make_where_clause();
            where_clause.predicates.push(parse_quote!(T: __MutatorTrait));
        }
    }
}

trait GenericItem {
    fn generics(&self) -> &Generics;
    fn generics_mut(&mut self) -> &mut Generics;
}

impl GenericItem for ItemStruct {
    fn generics(&self) -> &Generics {
        &self.generics
    }
    fn generics_mut(&mut self) -> &mut Generics {
        &mut self.generics
    }
}

impl GenericItem for ItemEnum {
    fn generics(&self) -> &Generics {
        &self.generics
    }
    fn generics_mut(&mut self) -> &mut Generics {
        &mut self.generics
    }
}

impl GenericItem for ItemUnion {
    fn generics(&self) -> &Generics {
        &self.generics
    }
    fn generics_mut(&mut self) -> &mut Generics {
        &mut self.generics
    }
}