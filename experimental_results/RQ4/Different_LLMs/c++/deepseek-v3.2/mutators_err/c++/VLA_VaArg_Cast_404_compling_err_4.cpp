//header file
#pragma once
#include "Mutator_base.h"

/**
 * VLA_VaArg_Cast_404
 */ 
class MutatorFrontendAction_404 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(404)

private:
    class MutatorASTConsumer_404 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_404(Rewriter &R) : TheRewriter(R) {}
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
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("CSCast")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto *SubExpr = MT->getSubExpr();
      if (!SubExpr)
        return;
      if (auto *CallExpr = dyn_cast<CallExpr>(SubExpr)) {
        auto *Callee = CallExpr->getCallee();
        if (!Callee)
          return;
        if (auto *DeclRefExpr = dyn_cast<DeclRefExpr>(Callee)) {
          auto *FuncDecl = dyn_cast<FunctionDecl>(DeclRefExpr->getDecl());
          if (!FuncDecl || FuncDecl->getNameAsString() != "va_arg")
            return;
        }
      }
      QualType DestType = MT->getTypeAsWritten();
      if (!DestType->isPointerType())
        return;
      auto *SubExprCall = dyn_cast<CallExpr>(SubExpr);
      if (!SubExprCall)
        return;
      auto *ArgExpr = SubExprCall->getArg(1);
      if (!ArgExpr)
        return;
      QualType ArgType = ArgExpr->getType();
      if (!ArgType->isArrayType())
        return;
      const ArrayType *ArrType = dyn_cast<ArrayType>(ArgType.getTypePtr());
      if (!ArrType)
        return;
      if (!ArrType->isVariableArrayType())
        return;
      //(optional)Record the node information to be used in the mutation process
      SourceRange CastRange = MT->getSourceRange();
      string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), CastRange);
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(1);
      string MutatedText;
      if (choice == 0) {
        // Mutation 1: Replace (DestType)va1_arg(args, ElementType[size]) with (DestType)&(va_arg(args, ElementType[size]))[0]
        string DestTypeStr = stringutils::rangetoStr(*(Result.SourceManager), MT->getTypeInfoAsWritten()->getTypeLoc().getSourceRange());
        string VaArgCallStr = stringutils::rangetoStr(*(Result.SourceManager), SubExprCall->getSourceRange());
        MutatedText = "(" + DestTypeStr + ")&(" + VaArgCallStr + ")[0]";
      } else {
        // Mutation 2: Replace (DestType)va_arg(args, ElementType[size]) with reinterpret_cast<DestType>(va_arg(args, ElementType[size]))
        string DestTypeStr = stringutils::rangetoStr(*(Result.SourceManager), MT->getTypeInfoAsWritten()->getTypeLoc().getSourceRange());
        string VaArgCallStr = stringutils::rangetoStr(*(Result.SourceManager), SubExprCall->getSourceRange());
        MutatedText = "reinterpret_cast<" + DestTypeStr + ">(" + VaArgCallStr + ")";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CastRange), MutatedText);
    }
}
  
void MutatorFrontendAction_404::MutatorASTConsumer_404::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cStyleCastExpr(hasSubExpr(callExpr(callee(functionDecl(hasName("va_arg")))))).bind("CSCast");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}