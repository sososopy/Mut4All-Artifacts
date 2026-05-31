//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Argument_With_Temporary_Object_167
 */ 
class MutatorFrontendAction_167 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(167)

private:
    class MutatorASTConsumer_167 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_167(Rewriter &R) : TheRewriter(R) {}
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
#define MUT167_OUTPUT 1

void MutatorFrontendAction_167::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PVD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("paramWithDefault")) {
      //Filter nodes in header files
      if (!PVD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PVD->getLocation()))
        return;
      if (!PVD->hasDefaultArg())
        return;
      auto *DefaultArg = PVD->getDefaultArg();
      if (!DefaultArg)
        return;
      // Check if default argument is a simple literal
      if (!isa<IntegerLiteral>(DefaultArg) && !isa<FloatingLiteral>(DefaultArg) &&
          !isa<CXXBoolLiteralExpr>(DefaultArg) && !isa<CXXNullPtrLiteralExpr>(DefaultArg))
        return;
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto DefaultArgRange = CharSourceRange::getTokenRange(DefaultArg->getSourceRange());
      std::string DefaultArgText = stringutils::rangetoStr(SM, DefaultArgRange);
      QualType ParamType = PVD->getType();
      //Perform mutation on the source code text by applying string replacement
      std::string NewDefaultArg;
      if (ParamType->isReferenceType()) {
        QualType PointeeType = ParamType->getPointeeType();
        if (!PointeeType.isNull()) {
          NewDefaultArg = PointeeType.getAsString() + "(" + DefaultArgText + ")";
        } else {
          NewDefaultArg = ParamType.getAsString() + "(" + DefaultArgText + ")";
        }
      } else {
        NewDefaultArg = ParamType.getAsString() + "(" + DefaultArgText + ")";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DefaultArgRange, NewDefaultArg);
    }
}
  
void MutatorFrontendAction_167::MutatorASTConsumer_167::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = parmVarDecl(hasDefaultArg()).bind("paramWithDefault");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}