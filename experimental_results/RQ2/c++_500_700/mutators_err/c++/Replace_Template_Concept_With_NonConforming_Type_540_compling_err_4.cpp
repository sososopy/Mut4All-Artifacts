//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_template_concept_with_nonconforming_type_540
 */ 
class MutatorFrontendAction_540 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(540)

private:
    class MutatorASTConsumer_540 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_540(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/replace_template_concept_with_nonconforming_type_540.h"
#include <regex>

// ========================================================================================================
#define MUT540_OUTPUT 1

void MutatorFrontendAction_540::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *TTPD = llvm::dyn_cast<clang::TemplateTypeParmDecl>(TD->getTemplatedDecl())) {
            if (TTPD->hasTypeConstraint()) {
                auto constraint = TTPD->getTypeConstraint();
                std::string replacement = "NonConformingType";
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), TTPD->getSourceRange());
                std::string mutatedText = std::regex_replace(originalText, std::regex(constraint->getImmediatelyDeclaredConstraint()->getType()->getCanonicalTypeInternal().getAsString()), replacement);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(TTPD->getSourceRange()), mutatedText);
            }
        }
    }
}
  
void MutatorFrontendAction_540::MutatorASTConsumer_540::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::decl(clang::ast_matchers::hasDescendant(clang::ast_matchers::templateTypeParmDecl(clang::ast_matchers::hasTypeConstraint()))).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}