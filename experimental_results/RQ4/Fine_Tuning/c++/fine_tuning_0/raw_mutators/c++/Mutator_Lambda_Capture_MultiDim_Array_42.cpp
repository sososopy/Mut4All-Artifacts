//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Lambda_Capture_MultiDim_Array_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Lambda_Capture_MultiDim_Array_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      //Get the source code text of target node
      auto body = FD->getBody();
      auto bodystr = stringutils::rangetoStr(*(Result.SourceManager),
                                             body->getSourceRange());
      auto lambdas = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas");
      if (!lambdas)
        return;
      auto captures = lambdas->capture_begin();
      for (auto capture : lambdas->captures()) {
        auto vardecl = capture.getCapturedVar();
        auto ty = vardecl->getType();
        if (ty->isArrayType() == false)
          continue;
        auto arrayty = ty->getAsArrayTypeUnsafe();
        if (arrayty->isSugared() == false)
          continue;
        auto arraystr = stringutils::rangetoStr(*(Result.SourceManager),
                                                vardecl->getSourceRange());
        llvm::outs() << arraystr << '\n';
        auto arraytystr = arrayty->desugar().getAsString();
        llvm::outs() << arraytystr << '\n';
        auto pos = bodystr.find(arraystr);
        if (pos == string::npos)
          continue;
        auto lbracket = bodystr.find('{', pos);
        auto rbracket = bodystr.find('}', pos);
        if (lbracket == string::npos || rbracket == string::npos)
          continue;
        auto arrayinit = bodystr.substr(lbracket, rbracket - lbracket + 1);
        llvm::outs() << arrayinit << '\n';
        auto arraytystr_mut = arraytystr + "[1]";
        auto arrayinit_mut = arrayinit.substr(0, arrayinit.size() - 1);
        arrayinit_mut += ",{";
        arrayinit_mut += arrayinit + "}}";
        llvm::outs() << arrayinit_mut << '\n';
        bodystr.replace(pos, arraystr.size(), arraytystr_mut);
        bodystr.replace(lbracket, arrayinit.size(), arrayinit_mut);
      }
      //Perform mutation on the source code text by applying string replacement
      bodystr = "/*mut42*/" + bodystr;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodystr);
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(lambdaExpr())).bind("Functions");
    auto lambdamatcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambdamatcher, &callback);
    matchFinder.matchAST(Context);
}