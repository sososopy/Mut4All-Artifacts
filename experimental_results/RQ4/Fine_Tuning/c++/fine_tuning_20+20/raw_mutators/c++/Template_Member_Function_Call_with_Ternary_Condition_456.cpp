//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Member_Function_Call_with_Ternary_Condition_456
 */ 
class MutatorFrontendAction_456 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(456)

private:
    class MutatorASTConsumer_456 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_456(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/template_member_function_call_with_ternary_condition_456.h"

// ========================================================================================================
#define MUT456_OUTPUT 1

void MutatorFrontendAction_456::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
    if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getBeginLoc()))
      return;
    cur_templates.push_back(TD);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isCompleteDefinition())
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("template") != string::npos)
      return;
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), "\n\t/*mut456*/void mut_456(){}\n");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isTemplateInstantiation() ||
        FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate)
      return;
    if (!FD->hasBody())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
    if (content.rfind('}') != string::npos)
      content.insert(content.rfind('}'), "\n\t/*mut456*/T.mut_456();\n");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), content);
  } else if (auto *TE = Result.Nodes.getNodeAs<clang::ConditionalOperator>(
                 "TernaryExpr")) {
    if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TE->getBeginLoc()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), TE->getSourceRange());
    if (content.find("mut_456") == string::npos)
      return;
    int choice = getrandom::getRandomIndex(cur_templates.size() - 1);
    auto target = cur_templates[choice];
    auto target_name = target->getNameAsString();
    llvm::outs() << target_name << '\n';
    auto mut_content = "0 ? 1 : " + target_name + "<C>{}";
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TE->getSourceRange()),
                        mut_content);
  }
}

void MutatorFrontendAction_456::MutatorASTConsumer_456::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = namedDecl(hasDescendant(templateDecl())).bind("Templates");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto function_matcher = functionDecl().bind("Functions");
    auto ternary_matcher = conditionalOperator().bind("TernaryExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.addMatcher(ternary_matcher, &callback);
    matchFinder.matchAST(Context);
}