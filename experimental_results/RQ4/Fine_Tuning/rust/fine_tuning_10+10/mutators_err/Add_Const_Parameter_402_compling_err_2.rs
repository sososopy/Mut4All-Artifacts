use syn::WherePredicate;

pub struct Add_Const_Parameter_402;

impl Mutator for Add_Const_Parameter_402 {
    fn name(&self) -> &str {
        "Add_Const_Parameter_402"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    let mut new_predicates: Vec<WherePredicate> = Vec::new();
                    for predicate in &where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let Some(const_generic) = predicate_type.bounds.iter().find_map(|bound| {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("Copy") {
                                        return Some(trait_bound);
                                    }
                                }
                                None
                            }) {
                                let new_const_ident = Ident::new("NM", Span::call_site());
                                let new_const_expr: Expr = parse_quote!({ NP - 2 });
                                let new_generic_param: GenericParam = parse_quote!(const #new_const_ident: usize = #new_const_expr);
                                
                                func.sig.generics.params.push(new_generic_param);
                                new_predicates.push(parse_quote!(#new_const_ident: Copy));
                            }
                        }
                    }
                    where_clause.predicates.extend(new_predicates);
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function signatures with generic type parameters and associated constraints. It identifies the `where` clause and introduces an additional `const` parameter involving arithmetic operations with existing type parameters. This new `const` creates an inter-dependency with existing constants, testing the compiler's handling of complex const expressions."
    }
}