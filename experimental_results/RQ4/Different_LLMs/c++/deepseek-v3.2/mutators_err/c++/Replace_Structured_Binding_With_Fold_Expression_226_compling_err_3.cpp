//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Structured_Binding_With_Fold_Expression_226
 */ 
class MutatorFrontendAction_226 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(226)

private:
    class MutatorASTConsumer_226 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_226(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SB = Result.Nodes.getNodeAs<clang::DecompositionDecl>("StructuredBinding")) {
      //Filter nodes in header files
      if (!SB || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SB->getLocation()))
        return;
      //Check that the structured binding is inside a templated entity
      auto *Parent = Result.Nodes.getNodeAs<clang::Decl>("TemplateParent");
      if (!Parent)
        return;
      //Get the source code text of target node
      auto SourceRange = SB->getSourceRange();
      auto OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      //Perform mutation on the source code text by applying string replacement
      //Replace the structured binding with a pack and add a fold expression
      std::string MutatedText = "const auto& [...pack] = ";
      //Find the expression part after the '=' in the original structured binding
      auto InitExprRange = SB->getInit()->getSourceRange();
      auto InitExprText = stringutils::rangetoStr(*(Result.SourceManager), InitExprRange2);
      MutatedText += InitExprText;
      MutatedText += ";\n(pack, ...);";
      MutatedText = "/*mut226*/" + MutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
}
  
void MutatorFrontendAction_226::MutatorASTConsumer_226::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = decompositionDecl(
      hasParent(
        decl(
          anyOf(
            functionDecl(isTemplate()),
            classTemplateDecl(),
            lambdaExpr(isGeneric()),
            cxxRecordDecl(isTemplate())
          )
        )
      )
    ).bind("StructuredBinding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}