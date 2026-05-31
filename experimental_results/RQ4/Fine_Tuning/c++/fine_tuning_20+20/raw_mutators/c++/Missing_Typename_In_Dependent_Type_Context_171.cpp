//header file
#pragma once
#include "Mutator_base.h"

/**
 * Missing_Typename_In_Dependent_Type_Context_171
 */ 
class MutatorFrontendAction_171 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(171)

private:
    class MutatorASTConsumer_171 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_171(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> candidate_funcs;
    };
};

//source file
#include "../include/Missing_Typename_In_Dependent_Type_Context_171.h"

// ========================================================================================================
#define MUT171_OUTPUT 1

void MutatorFrontendAction_171::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Functions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      candidate_funcs.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) { // Find the class with nested types
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Tag) {
          llvm::outs() << DL->getNameAsString() << '\n';
          auto name = DL->getNameAsString();
          for (auto func : candidate_funcs) {
            if (stringutils::rangetoStr(*(Result.SourceManager),
                                        func->getSourceRange())
                    .find(name) != string::npos) {
              auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                     func->getSourceRange());
              if (content.find("typename") != string::npos) {
                content.replace(content.find("typename"), 8, "");
                Rewrite.ReplaceText(
                    CharSourceRange::getTokenRange(func->getSourceRange()),
                    content);
              }
            }
          }
        }
      }
    }
  }
  
void MutatorFrontendAction_171::MutatorASTConsumer_171::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Functions");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}