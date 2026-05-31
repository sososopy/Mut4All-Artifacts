//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Template_Alias_412
 */ 
class MutatorFrontendAction_412 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(412)

private:
    class MutatorASTConsumer_412 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_412(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> cur_decls;
    };
};

//source file
#include "../include/Expand_Template_Alias_412.h"

// ========================================================================================================
#define MUT412_OUTPUT 1

void MutatorFrontendAction_412::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (TD->getKind() == Decl::Kind::ClassTemplate) {
        auto CT = dyn_cast<ClassTemplateDecl>(TD);
        if (CT->getTemplateParameters()->size() == 1)
          return;
      } else if (TD->getKind() == Decl::Kind::FunctionTemplate) {
        auto FT = dyn_cast<FunctionTemplateDecl>(TD);
        if (FT->getTemplateParameters()->size() == 1)
          return;
      } else if (TD->getKind() == Decl::Kind::TypeAliasTemplate) {
        auto AL = dyn_cast<TypeAliasTemplateDecl>(TD);
        if (AL->getTemplateParameters()->size() == 1)
          return;
      } else
        return;
      cur_decls.push_back(TD);
    } else if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>(
                   "AliasUsage")) {
      if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AL->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             AL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("...") == string::npos)
        return;
      content.insert(content.rfind('>'), ", int");
      content = "/*mut412*/" + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AL->getSourceRange()),
                          content);
    }
  }
  
void MutatorFrontendAction_412::MutatorASTConsumer_412::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateDecl().bind("Templates");
    auto alias_usage_matcher =
        typeAliasDecl(hasType(templateSpecializationType())).bind("AliasUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(alias_usage_matcher, &callback);
    matchFinder.matchAST(Context);
}