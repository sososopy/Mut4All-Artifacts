use crate::mutator::Mutator;

pub struct Enum_Const_Expr_Cycle_140;

impl Mutator for Enum_Const_Expr_Cycle_140 {
    fn name(&self) -> &str {
        "Enum_Const_Expr_Cycle_140"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut modified_variants = Vec::new();
                for variant in &item_enum.variants {
                    if let Some((_, Expr::Path(expr_path))) = &variant.discriminant {
                        if expr_path.path.segments.iter().any(|seg| seg.ident == item_enum.ident) {
                            modified_variants.push(variant.clone());
                        }
                    }
                }
                if !modified_variants.is_empty() {
                    for variant in &mut item_enum.variants {
                        if let Some((_, Expr::Path(expr_path))) = &variant.discriminant {
                            if expr_path.path.segments.iter().any(|seg| seg.ident == item_enum.ident) {
                                let variant_ident = &variant.ident;
                                let new_expr: Expr = parse_quote! {
                                    #item_enum::#variant_ident as u8 - 1
                                };
                                variant.discriminant = Some((token::Eq::default(), new_expr));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with constant expressions involving their own variants. It identifies variants using arithmetic expressions referencing the enum's variants and modifies them to create a cycle or undefined behavior, such as through subtraction or other arithmetic operations."
    }
}