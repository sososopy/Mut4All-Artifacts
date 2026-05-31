//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Template_Instantiation_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/recursive_template_instantiation_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      auto &Context = *Result.Context;
      auto &SM = Context.getSourceManager();
      auto &LangOpts = Context.getLangOpts();

      // Get the source code text of the target node
      auto templateDeclStr = stringutils::rangetoStr(SM, TD->getSourceRange());

      // Perform mutation on the source code text by applying string replacement
      std::string mutatedTemplate = templateDeclStr;
      std::string::size_type pos = mutatedTemplate.find("template<");
      if (pos != std::string::npos) {
        mutatedTemplate.insert(pos + 9, "int N, ");
        std::string nestedType = "using Nested = " + TD->getNameAsString() + "<N-1, " + TD->getNameAsString() + "<N-1, T>>;";
        std::string::size_type classPos = mutatedTemplate.find("class");
        if (classPos != std::string::npos) {
          mutatedTemplate.insert(mutatedTemplate.find("{", classPos) + 1, "\n" + nestedType + "\n");
        }
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), mutatedTemplate);
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}