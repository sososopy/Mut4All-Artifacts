//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Reference_Member_To_Derived_Struct_With_Variant_82
 */ 
class MutatorFrontendAction_82 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(82)

private:
    class MutatorASTConsumer_82 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_82(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const CXXRecordDecl *> derived_classes;
        std::vector<const CXXRecordDecl *> base_classes;
    };
};

//source file
#include "../include/Add_Reference_Member_To_Derived_Struct_With_Variant_82.h"

// ========================================================================================================
#define MUT82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("derived")) {
        if (!derived || !Result.Context->getSourceManager().isWrittenInMainFile(derived->getLocation()))
            return;
        if (derived->isLambda() || !derived->isCompleteDefinition())
            return;

        const CXXRecordDecl *base = derived->getBaseClass(0);
        if (!base)
            return;

        std::string ref_member = "int& mut_ref_82;";
        SourceLocation end_loc = derived->getEndLoc();
        Rewrite.InsertTextAfterToken(end_loc, "\n    " + ref_member);

        derived_classes.push_back(derived);
        base_classes.push_back(base);
    }
    else if (auto *var_decl = Result.Nodes.getNodeAs<clang::VarDecl>("variant")) {
        if (!var_decl || !Result.Context->getSourceManager().isWrittenInMainFile(var_decl->getLocation()))
            return;

        QualType var_type = var_decl->getType();
        if (auto *ts_type = var_type->getAs<TemplateSpecializationType>()) {
            TemplateName tname = ts_type->getTemplateName();
            if (tname.getAsQualifiedTemplateName()->getIdentifier().getName() != "std::variant")
                return;

            for (unsigned i = 0; i < ts_type->getNumArgs(); ++i) {
                auto arg = ts_type->getTypeArgument(i, 0);
                if (auto *record_type = arg->getAs<RecordType>()) {
                    const CXXRecordDecl *record_decl = record_type->getDecl()->getAsCXXRecordDecl();
                    if (record_decl && std::find(base_classes.begin(), base_classes.end(), record_decl) != base_classes.end()) {
                        std::string new_variant = "std::variant<";
                        for (unsigned j = 0; j < ts_type->getNumArgs(); ++j) {
                            if (j > 0) new_variant += ", ";
                            new_variant += ts_type->getTypeArgument(j, 0).getAsString();
                        }
                        auto base_iter = std::find(base_classes.begin(), base_classes.end(), record_decl);
                        if (base_iter != base_classes.end()) {
                            size_t index = base_iter - base_classes.begin();
                            const CXXRecordDecl *derived = derived_classes[index];
                            new_variant += ", " + derived->getNameAsString();
                        }
                        new_variant += "> v = " + record_decl->getNameAsString() + "{};";

                        SourceRange range = var_decl->getSourceRange();
                        Rewrite.ReplaceText(range, new_variant);
                        break;
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher derived_matcher = cxxRecordDecl(hasBaseClass(cxxRecordDecl())).bind("derived");
    DeclarationMatcher variant_matcher = varDecl(hasType(templateSpecializationType(
        hasTemplateName(qualifiedTemplateName(hasName("std::variant"))),
        anything()
    ))).bind("variant");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(variant_matcher, &callback);
    matchFinder.matchAST(Context);
}