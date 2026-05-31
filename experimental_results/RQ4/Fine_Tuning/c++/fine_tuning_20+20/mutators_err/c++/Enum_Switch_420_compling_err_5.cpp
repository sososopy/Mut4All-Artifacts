//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Switch_420
 */ 
class MutatorFrontendAction_420 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(420)

private:
    class MutatorASTConsumer_420 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_420(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::EnumDecl *> cur_enums;
    };
};

//source file
#include "../include/enum_switch_420.h"

// ========================================================================================================
#define MUT420_OUTPUT 1

void MutatorFrontendAction_420::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::SwitchStmt>("Switch")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    auto cond = DL->getCond();
    if (cond == nullptr)
      return;
    auto cond_type = cond->getType();
    if (cond_type->isEnumeralType() == false)
      return;
    auto enumtype = cond_type->getAs<EnumType>();
    if (enumtype == nullptr)
      return;
    auto enumdecl = enumtype->getDecl();
    if (enumdecl == nullptr)
      return;
    auto cases = DL->getBody()->children();
    bool has_enum_case = false;
    for (auto cs : cases) {
      if (cs->getStmtClass() == Stmt::CaseStmtClass) {
        auto casestmt = cast<CaseStmt>(cs);
        auto caseval = casestmt->getLHS();
        if (caseval->getType() == cond_type) {
          has_enum_case = true;
          break;
        }
      }
    }
    if (has_enum_case == false) {
      auto enumerators = enumdecl->enumerators();
      if (enumerators.empty() == false) {
        auto en = enumerators.begin();
        auto ename = (*en)->getNameAsString();
        Rewrite.ReplaceText(DL->getEndLoc(), 0,
                            "\n/*mut420*/case " + ename + ":break;\n");
      }
    }
    bool has_default = false;
    for (auto cs : cases) {
      if (cs->getStmtClass() == Stmt::DefaultStmtClass) {
        has_default = true;
        break;
      }
    }
    if (has_default == false) {
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "\n/*mut420*/default:break;\n");
    }
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    cur_enums.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::SwitchStmt>(
                 "SwitchWithoutEnum")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    if (cur_enums.empty()) {
      Rewrite.ReplaceText(DL->getBeginLoc(), 0,
                          "/*mut420*/enum TestEnum{EnumValue1};\n");
      return;
    }
    auto index = getrandom::getRandomIndex(cur_enums.size() - 1);
    auto target_enum = cur_enums[index];
    auto enumerators = target_enum->enumerators();
    if (enumerators.empty()) {
      Rewrite.ReplaceText(DL->getBeginLoc(), 0,
                          "/*mut420*/enum TestEnum{EnumValue1};\n");
      return;
    }
    auto en = enumerators.begin();
    auto ename = (*en)->getNameAsString();
    auto cond = DL->getCond();
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(cond->getSourceRange()),
                        ename);
  }
}
  
void MutatorFrontendAction_420::MutatorASTConsumer_420::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = switchStmt().bind("Switch");
  auto enum_matcher = enumDecl().bind("Enums");
  auto switch_without_enum_matcher =
      switchStmt(unless(hasCondition(hasType(unless(hasType(hasCanonicalType(hasCanonicalType(hasCanonicalType(isEnumeral())))))))))
          .bind("SwitchWithoutEnum");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(enum_matcher, &callback);
  matchFinder.addMatcher(switch_without_enum_matcher, &callback);
  matchFinder.matchAST(Context);
}