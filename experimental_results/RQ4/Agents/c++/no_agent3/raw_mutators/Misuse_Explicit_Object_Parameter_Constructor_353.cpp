//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Explicit_Object_Parameter_Constructor_353
 */ 
class MutatorFrontendAction_353 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(353)

private:
    class MutatorASTConsumer_353 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_353(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Misuse_Explicit_Object_Parameter_Constructor_353.h"

// ========================================================================================================
#define MUT353_OUTPUT 1

void MutatorFrontendAction_353::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (CD->isImplicit())
        return;

      //Get the source code text of target node
      auto constructorText = stringutils::rangetoStr(*(Result.SourceManager),
                                                     CD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedConstructor = "/*mut353*/" + constructorText;
      size_t pos = mutatedConstructor.find("(");
      if (pos != std::string::npos) {
        mutatedConstructor.insert(pos + 1, "this auto &self, ");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), mutatedConstructor);
    }
}
  
void MutatorFrontendAction_353::MutatorASTConsumer_353::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}