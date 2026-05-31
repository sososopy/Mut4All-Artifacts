//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_And_Typedef_In_Control_Structures_421
 */ 
class MutatorFrontendAction_421 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(421)

private:
    class MutatorASTConsumer_421 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_421(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TypedefDecl *target_typedef;
        std::string target_typedef_name;
    };
};

//source file
#include "../include/Struct_And_Typedef_In_Control_Structures_421.h"

// ========================================================================================================
#define MUT421_OUTPUT 1

void MutatorFrontendAction_421::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedefs")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      llvm::outs() << TD->getNameAsString() << "\n";
      target_typedef = TD;
      target_typedef_name = TD->getNameAsString();
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isMain() == false)
        return;
      auto forstmt = clang::ast_matchers::forStmt().bind("For");
      auto ifstmt = clang::ast_matchers::ifStmt().bind("If");
      auto whilestmt = clang::ast_matchers::whileStmt().bind("While");
      auto switchstmt = clang::ast_matchers::switchStmt().bind("Switch");
      auto doStmt = clang::ast_matchers::doStmt().bind("Do");
      auto caseStmt = clang::ast_matchers::caseStmt().bind("Case");
      auto compoundStmt = clang::ast_matchers::compoundStmt().bind("Compound");
      auto matchers = clang::ast_matchers::statementCountIs(1);
      auto finder = clang::ast_matchers::match(matchers, *FD->getBody(), *Result.Context);
      for (auto &f : finder) {
        if (auto *FS = f.getNodeAs<clang::ForStmt>("For")) {
          llvm::outs() << "Find forstmt\n";
          if (FS->getInit() == nullptr) {
            auto source_range = FS->getSourceRange();
            auto source_range_str = stringutils::rangetoStr(
                *(Result.SourceManager), source_range);
            llvm::outs() << source_range_str << "\n";
            auto struct_def = " struct " + target_typedef_name + "{int x;}";
            source_range_str.insert(source_range_str.find('(') + 1,
                                    struct_def);
            llvm::outs() << source_range_str << "\n";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(source_range),
                                source_range_str);
            break;
          }
        } else if (auto *IS = f.getNodeAs<clang::IfStmt>("If")) {
          llvm::outs() << "Find ifstmt\n";
          if (IS->getInit() == nullptr) {
            auto source_range = IS->getSourceRange();
            auto source_range_str = stringutils::rangetoStr(
                *(Result.SourceManager), source_range);
            llvm::outs() << source_range_str << "\n";
            auto struct_def = " struct " + target_typedef_name + "{int x;}";
            source_range_str.insert(source_range_str.find('(') + 1,
                                    struct_def);
            llvm::outs() << source_range_str << "\n";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(source_range),
                                source_range_str);
            break;
          }
        } else if (auto *WS = f.getNodeAs<clang::WhileStmt>("While")) {
          llvm::outs() << "Find whilestmt\n";
          auto source_range = WS->getSourceRange();
          auto source_range_str = stringutils::rangetoStr(
              *(Result.SourceManager), source_range);
          llvm::outs() << source_range_str << "\n";
          auto struct_def = " struct " + target_typedef_name + "{int x;}";
          source_range_str.insert(source_range_str.find('(') + 1, struct_def);
          llvm::outs() << source_range_str << "\n";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(source_range),
                              source_range_str);
          break;
        } else if (auto *SS = f.getNodeAs<clang::SwitchStmt>("Switch")) {
          llvm::outs() << "Find switchstmt\n";
          auto source_range = SS->getSourceRange();
          auto source_range_str = stringutils::rangetoStr(
              *(Result.SourceManager), source_range);
          llvm::outs() << source_range_str << "\n";
          auto struct_def = " struct " + target_typedef_name + "{int x;}";
          source_range_str.insert(source_range_str.find('(') + 1, struct_def);
          llvm::outs() << source_range_str << "\n";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(source_range),
                              source_range_str);
          break;
        } else if (auto *DS = f.getNodeAs<clang::DoStmt>("Do")) {
          llvm::outs() << "Find dostmt\n";
          auto source_range = DS->getSourceRange();
          auto source_range_str = stringutils::rangetoStr(
              *(Result.SourceManager), source_range);
          llvm::outs() << source_range_str << "\n";
          auto struct_def = " struct " + target_typedef_name + "{int x;}";
          source_range_str.insert(source_range_str.find('(') + 1, struct_def);
          llvm::outs() << source_range_str << "\n";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(source_range),
                              source_range_str);
          break;
        } else if (auto *CS = f.getNodeAs<clang::CaseStmt>("Case")) {
          llvm::outs() << "Find casestmt\n";
          auto source_range = CS->getSourceRange();
          auto source_range_str = stringutils::rangetoStr(
              *(Result.SourceManager), source_range);
          llvm::outs() << source_range_str << "\n";
          auto struct_def = " struct " + target_typedef_name + "{int x;}";
          source_range_str.insert(source_range_str.find(':') + 1, struct_def);
          llvm::outs() << source_range_str << "\n";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(source_range),
                              source_range_str);
          break;
        } else if (auto *CS = f.getNodeAs<clang::CompoundStmt>("Compound")) {
          llvm::outs() << "Find compoundstmt\n";
          auto source_range = CS->getSourceRange();
          auto source_range_str = stringutils::rangetoStr(
              *(Result.SourceManager), source_range);
          llvm::outs() << source_range_str << "\n";
          auto struct_def = " struct " + target_typedef_name + "{int x;}";
          source_range_str.insert(source_range_str.find('{') + 1, struct_def);
          llvm::outs() << source_range_str << "\n";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(source_range),
                              source_range_str);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_421::MutatorASTConsumer_421::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto typedef_matcher = clang::ast_matchers::typedefDecl().bind("Typedefs");
    auto function_matcher = clang::ast_matchers::functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typedef_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}