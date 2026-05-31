//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Self_Type_Member_201
 */ 
class MutatorFrontendAction_201 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(201)

private:
    class MutatorASTConsumer_201 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_201(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Self_Type_Member_201.h"

// ========================================================================================================
#define MUT201_OUTPUT 1

void MutatorFrontendAction_201::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!DL->isCompleteDefinition())
        return;
      auto classname = DL->getNameAsString();
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto fielddecl = "\n\t/*mut201*/" + classname + " a;\n";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), fielddecl);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_201::MutatorASTConsumer_201::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}