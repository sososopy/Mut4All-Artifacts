//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Redefinition_In_Template_Class_Context_433
 */ 
class MutatorFrontendAction_433 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(433)

private:
    class MutatorASTConsumer_433 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_433(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl *> cur_enums;
    };
};

//source file
#include "../include/enum_redefinition_in_template_class_context_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isLambda() || DL->isStruct() || DL->isClass() == false)
      return;
    if (DL->isTemplated() == false)
      return;
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          clang::Decl::IdentifierNamespace::IDNS_Enum) {
        auto enumdecl = dyn_cast<clang::EnumDecl>(decl);
        cur_enums.push_back(enumdecl);
      }
    }
  } else if (auto *EL =
                 Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
    if (!EL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   EL->getLocation()))
      return;
    if (cur_enums.empty())
      return;
    auto target = cur_enums[getrandom::getRandomIndex(cur_enums.size() - 1)];
    auto target_enum = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
    string new_class = "/*mut433*/template <typename U> class Mut433 {\n" +
                       target_enum + "\n};\n";
    Rewrite.ReplaceText(EL->getSourceRange(), new_class);
  }
}
  
void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto enum_matcher = enumDecl().bind("Enums");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(enum_matcher, &callback);
  matchFinder.matchAST(Context);
}