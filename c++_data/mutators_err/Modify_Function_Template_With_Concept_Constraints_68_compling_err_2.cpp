//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Template_With_Concept_Constraints_68
 */ 
class MutatorFrontendAction_68 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(68)

private:
    class MutatorASTConsumer_68 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_68(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_function_template_with_concept_constraints_68.h"

// ========================================================================================================
#define MUT68_OUTPUT 1

void MutatorFrontendAction_68::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        if (auto *CT = FT->getTemplatedDecl()) {
            // Assuming constraints are represented by a specific attribute or similar
            if (CT->hasAttr<clang::ConceptAttr>()) {
                auto constraints = CT->getAttr<clang::ConceptAttr>();
                std::string newConcept = "RecursiveConcept";
                std::string originalConcept = constraints->getSpelling();
                std::string replacement = "template<typename T> concept " + newConcept + " = " + originalConcept + " && requires(T t) { anotherFunction(t); };";

                std::string functionTemplateText = stringutils::rangetoStr(*(Result.SourceManager), FT->getSourceRange());
                size_t pos = functionTemplateText.find(originalConcept);
                if (pos != std::string::npos) {
                    functionTemplateText.replace(pos, originalConcept.length(), newConcept);
                    functionTemplateText = replacement + "\n" + functionTemplateText;
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), functionTemplateText);
                }
            }
        }
    }
}

void MutatorFrontendAction_68::MutatorASTConsumer_68::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}