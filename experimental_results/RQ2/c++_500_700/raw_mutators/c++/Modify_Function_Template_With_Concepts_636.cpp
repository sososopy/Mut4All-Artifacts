//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_template_with_concepts_636
 */ 
class MutatorFrontendAction_636 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(636)

private:
    class MutatorASTConsumer_636 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_636(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_636.h"

// ========================================================================================================
#define MUT636_OUTPUT 1

void MutatorFrontendAction_636::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplatedDecl()->getBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());

        std::string newParam = ", typename U = int";
        std::string newConcept = "requires can_call<T, U> ";
        std::string newUsage = "U u; obj(u);";

        size_t pos = declaration.find(">");
        if (pos != std::string::npos) {
            declaration.insert(pos, newParam);
        }

        pos = declaration.find("{");
        if (pos != std::string::npos) {
            declaration.insert(pos + 1, newConcept);
            declaration.insert(pos + 1, newUsage);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_636::MutatorASTConsumer_636::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}