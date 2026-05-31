//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Nonmember_Comparison_To_Default_185
 */ 
class MutatorFrontendAction_185 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(185)

private:
    class MutatorASTConsumer_185 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_185(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/mutate_nonmember_comparison_to_default_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    cur_classes.push_back(CL);
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>( "Friend")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile( FD->getLocation())) return;
    if (cur_classes.empty())
      return;
    auto target = cur_classes.back();
    if (target->hasDefinition() == false)
      return;
    auto target_name = target->getNameAsString();
    auto fd_name = FD->getNameAsString();
    if (fd_name != "operator<=>")
      return;
    auto params = FD->parameters();
    if (params.size() != 2)
      return;
    auto param1_type = params[0]->getType().getAsString();
    auto param2_type = params[1]->getType().getAsString();
    if (param1_type != param2_type || param1_type != target_name)
      return;
    llvm::outs() << "Mut185: " << fd_name << " " << param1_type << " "
                 << param2_type << '\n';
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
    llvm::outs() << content << '\n';
    auto replacement = "/*mut185*/=default";
    if (content.rfind('}') != string::npos)
      content.replace(content.rfind('}'), 1, replacement);
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                        content);
  }
}
  
void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto friend_matcher = functionDecl(hasAttr(attr::Friend)).bind("Friend");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(friend_matcher, &callback);
  matchFinder.matchAST(Context);
}