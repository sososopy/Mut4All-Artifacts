//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Consteval_Ctor_To_Constinit_96
 */ 
class MutatorFrontendAction_96 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(96)

private:
    class MutatorASTConsumer_96 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_96(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/change_consteval_ctor_to_constinit_96.h"

// ========================================================================================================
#define MUT96_OUTPUT 1

void MutatorFrontendAction_96::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_classes.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "ConstinitClasses")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile( DL->getLocation())) return;
    if (DL->isCompleteDefinition() == false) return;
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->isCXXClassMember()) {
        if (decl->getAsFunction()) {
          auto func = decl->getAsFunction();
          if (func->isConsteval()) {
            auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   func->getSourceRange());
            llvm::outs() << content << '\n';
            if (content.find(':') == string::npos) return;
            if (content.find('{') == string::npos) return;
            auto colon_pos = content.find(':');
            auto lbrace_pos = content.find('{');
            if (colon_pos > lbrace_pos) return;
            auto init_list = content.substr(colon_pos, lbrace_pos - colon_pos);
            llvm::outs() << init_list << '\n';
            content.erase(colon_pos, lbrace_pos - colon_pos);
            content.insert(lbrace_pos, init_list);
            content = "/*mut96*/" + content;
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(func->getSourceRange()), content);
          }
        }
      }
    }
  }
}

void MutatorFrontendAction_96::MutatorASTConsumer_96::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto constinit_matcher =
      cxxRecordDecl(has(cxxConstructorDecl(hasAnyConstructorInitializer(
                           cxxCtorInitializer(isMemberInitializer())
                               .withInitializer(expr(hasDescendant(
                                   declRefExpr(to(varDecl(hasType(qualType(
                                       isConstinitQualified()))))))))))))
          .bind("ConstinitClasses");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(constinit_matcher, &callback);
  matchFinder.matchAST(Context);
}