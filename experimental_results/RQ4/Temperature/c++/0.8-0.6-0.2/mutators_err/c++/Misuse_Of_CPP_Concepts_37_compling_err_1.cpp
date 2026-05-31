//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_of_cpp_concepts_37
 */ 
class MutatorFrontendAction_37 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(37)

private:
    class MutatorASTConsumer_37 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_37(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_of_cpp_concepts_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                  TD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string conceptStr = "template <typename T>\nconcept UndefinedConcept = requires(T t) { t.undefinedMethod(); };\n";
      std::string requiresStr = "requires UndefinedConcept<T> ";
      
      // Insert concept definition
      templateDecl = conceptStr + templateDecl;
      
      // Insert requires clause
      size_t pos = templateDecl.find("class");
      if (pos != std::string::npos) {
        templateDecl.insert(pos, requiresStr);
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDecl);
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}