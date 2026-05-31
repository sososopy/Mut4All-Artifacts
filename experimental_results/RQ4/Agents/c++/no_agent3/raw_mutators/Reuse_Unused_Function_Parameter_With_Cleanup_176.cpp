//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reuse_Unused_Function_Parameter_With_Cleanup_176
 */ 
class MutatorFrontendAction_176 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(176)

private:
    class MutatorASTConsumer_176 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_176(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Reuse_Unused_Function_Parameter_With_Cleanup_176.h"

// ========================================================================================================
#define MUT176_OUTPUT 1

void MutatorFrontendAction_176::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithUnusedParam")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        for (auto param : FD->parameters()) {
          if (!param->getUsed()) {
            //Get the source code text of target node
            auto bodyRange = FD->getBody()->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
            //Perform mutation on the source code text by applying string replacement
            std::string paramName = param->getNameAsString();
            std::string mutation = "\nT tmp = std::move(" + paramName + ");\n";
            bodyText.insert(bodyText.find("{") + 1, mutation);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_176::MutatorASTConsumer_176::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(unless(hasType(autoType())), unless(parmVarDecl(hasType(referenceType()))))).bind("FunctionWithUnusedParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}