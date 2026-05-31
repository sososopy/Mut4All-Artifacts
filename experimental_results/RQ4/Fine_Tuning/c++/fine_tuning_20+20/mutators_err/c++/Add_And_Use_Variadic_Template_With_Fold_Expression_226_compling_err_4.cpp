//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_And_Use_Variadic_Template_With_Fold_Expression_226
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
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionDecl *> func_decls;
        std::vector<const clang::CXXRecordDecl *> class_decls;
    };
};

//source file
#include "../include/Add_And_Use_Variadic_Template_With_Fold_Expression_226.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isMain())
        return;
      if (FD->isTemplated())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isOverloadedOperator())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isDefaulted())
        return;
      if (FD->isDependentContext())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefined() == false)
        return;
      func_decls.push_back(FD);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      class_decls.push_back(DL);
    } else if (auto *SL =
                   Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TU")) {
      if (!SL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SL->getBeginLoc()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), SL->getSourceRange());
      if (content.find("main") == string::npos)
        return;
      if (content.find("template") != string::npos)
        return;
      if (content.find("...") != string::npos)
        return;
      if (content.find("fold") != string::npos)
        return;
      if (content.find("Fold") != string::npos)
        return;
      if (content.find("FOLD") != string::npos)
        return;
      if (content.find("FOLD") != string::npos)
        return;
      if (content.find("Fold") != string::npos)
        return;
      if (content.find("fold") != string::npos)
        return;
      if (content.find("...") != string::npos)
        return;
      if (content.find("template") != string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::npos)
        return;
      if (content.find("main") == string::