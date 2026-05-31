//header file
#pragma once
#include "Mutator_base.h"

/**
 * Parameter_Dependent_Requires_in_Out_of_Line_Definition_107
 */ 
class MutatorFrontendAction_107 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(107)

private:
    class MutatorASTConsumer_107 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_107(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_funcs;
    };
};

//source file
#include "../include/Parameter_Dependent_Requires_in_Out_of_Line_Definition_107.h"

// ========================================================================================================
#define MUT107_OUTPUT 1

void MutatorFrontendAction_107::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (!DL->isTemplated())
        return;
      cur_classes.push_back(DL);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Requires")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isInIdentifierNamespace(Decl::IdentifierNamespace::IDNS_Ordinary))
        return;
      if (!FD->isTemplated())
        return;
      if (!FD->isOutOfLine())
        return;
      if (FD->getMinRequiredArguments() == 0)
        return;
      if (FD->getNumParams() == 0)
        return;
      if (!FD->hasBody())
        return;
      if (!FD->hasTrailingRequiresClause())
        return;
      auto requires_clause = FD->getTrailingRequiresClause();
      if (requires_clause == nullptr)
        return;
      cur_funcs.push_back(FD);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("RequiresIn")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isInIdentifierNamespace(Decl::IdentifierNamespace::IDNS_Ordinary))
        return;
      if (!FD->isTemplated())
        return;
      if (FD->getMinRequiredArguments() == 0)
        return;
      if (FD->getNumParams() == 0)
        return;
      if (FD->hasBody())
        return;
      if (!FD->hasTrailingRequiresClause())
        return;
      auto requires_clause = FD->getTrailingRequiresClause();
      if (requires_clause == nullptr)
        return;
      cur_funcs.push_back(FD);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "ClassesIn")) { // If there is a return 0 statement in the
                                   // function, replace the return value with
                                   // the inserted static const
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (!DL->isTemplated())
        return;
      cur_classes.push_back(DL);
    }
  }
  
void MutatorFrontendAction_107::MutatorASTConsumer_107::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto matcher2 = functionDecl(hasTrailingRequiresClause()).bind("Requires");
    auto matcher3 = functionDecl(hasTrailingRequiresClause()).bind("RequiresIn");
    auto matcher4 = cxxRecordDecl().bind("ClassesIn");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.addMatcher(matcher4, &callback);
    matchFinder.matchAST(Context);
}