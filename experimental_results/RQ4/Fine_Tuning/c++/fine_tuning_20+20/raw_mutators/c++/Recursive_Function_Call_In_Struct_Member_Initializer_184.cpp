//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Function_Call_In_Struct_Member_Initializer_184
 */ 
class MutatorFrontendAction_184 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(184)

private:
    class MutatorASTConsumer_184 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_184(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *cur_struct;
    };
};

//source file
#include "../include/Recursive_Function_Call_In_Struct_Member_Initializer_184.h"

// ========================================================================================================
#define MUT184_OUTPUT 1

void MutatorFrontendAction_184::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isStruct())
        return;
      cur_struct = DL;
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("Field")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasInClassInitializer())
        return;
      auto init = FD->getInClassInitializer();
      if (init == nullptr)
        return;
      auto content = stringutils::getStmtStr(*(Result.SourceManager), init);
      if (content.find('(') == string::npos)
        return;
      if (content.find(')') == string::npos)
        return;
      if (content.find(cur_struct->getNameAsString()) == string::npos)
        return;
      auto new_content = content.substr(0, content.find('(')) +
                         "(" + cur_struct->getNameAsString() + "())";
      llvm::outs() << new_content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(init->getSourceRange()),
                          "/*mut184*/" + new_content);
    }
}
  
void MutatorFrontendAction_184::MutatorASTConsumer_184::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Struct");
    auto field_matcher = fieldDecl().bind("Field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(field_matcher, &callback);
    matchFinder.matchAST(Context);
}