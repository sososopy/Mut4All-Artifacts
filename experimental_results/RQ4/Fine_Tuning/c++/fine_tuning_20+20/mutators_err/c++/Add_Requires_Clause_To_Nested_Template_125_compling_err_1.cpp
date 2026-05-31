//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Requires_Clause_To_Nested_Template_125
 */ 
class MutatorFrontendAction_125 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(125)

private:
    class MutatorASTConsumer_125 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_125(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Add_Requires_Clause_To_Nested_Template_125.h"

// ========================================================================================================
#define MUT125_OUTPUT 1

void MutatorFrontendAction_125::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
          return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
          return;
        cur_classes.push_back(CL);
    } else if (auto *TL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "Templates")) {
        if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TL->getLocation()))
          return;
        if (cur_classes.empty())
          return;
        auto parent = TL->getParent();
        if (parent == nullptr)
          return;
        if (parent != cur_classes.back())
          return;
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               TL->getSourceRange());
        auto parent_name = parent->getNameAsString();
        if (content.find("requires") == string::npos)
          content.insert(content.find('>') + 1, " requires(" + parent_name + ")");
        else
          return;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_125::MutatorASTConsumer_125::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto template_matcher = functionTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}