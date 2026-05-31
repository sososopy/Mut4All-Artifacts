//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_New_Paren_Init_With_Brace_Init_For_Aggregate_99
 */ 
class MutatorFrontendAction_99 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(99)

private:
    class MutatorASTConsumer_99 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_99(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Replace_New_Paren_Init_With_Brace_Init_For_Aggregate_99.h"

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL->isAggregate())
        cur_classes.push_back(CL);
    } else if (auto *NE =
                   Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NE->getBeginLoc()))
        return;
      if (NE->getNumPlacementArgs() != 0)
        return;
      if (NE->isArray())
        return;
      if (NE->isListInitialization())
        return;
      auto type = NE->getAllocatedType();
      if (type->isBuiltinType())
        return;
      auto type_name = type.getAsString();
      for (auto cur_class : cur_classes) {
        if (cur_class->getNameAsString() == type_name) {
          auto args = NE->getConstructorArgs();
          if (args.size() == 0)
            return;
          auto args_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                   NE->getSourceRange());
          llvm::outs() << args_text << '\n';
          args_text.insert(args_text.find('(') + 1, "{");
          args_text.insert(args_text.rfind(')'), "}");
          llvm::outs() << args_text << '\n';
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(NE->getSourceRange()), args_text);
          return;
        }
      }
    }
}
  
void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto newexpr_matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(newexpr_matcher, &callback);
    matchFinder.matchAST(Context);
}