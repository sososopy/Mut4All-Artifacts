//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_In_Decltype_With_Unevaluated_Context_71
 */ 
class MutatorFrontendAction_71 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(71)

private:
    class MutatorASTConsumer_71 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_71(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::DecltypeType *> cur_decltypes;
    };
};

//source file
#include "../include/lambda_in_decltype_with_unevaluated_context_71.h"

// ========================================================================================================
#define MUT71_OUTPUT 1

void MutatorFrontendAction_71::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DT = Result.Nodes.getNodeAs<clang::DecltypeType>("Decltypes")) {
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getBeginLoc()))
        return;
      if (DT->isSugared())
        return;
      if (auto *DL = dyn_cast<DecltypeType>(DT->desugar())) {
        if (DL->getUnderlyingType()->isLambdaType())
          cur_decltypes.push_back(DT);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclStmt>("Declstmt")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (cur_decltypes.empty())
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (auto *VD = dyn_cast<VarDecl>(decl)) {
          if (VD->getType()->isDecltypeType()) {
            auto type = VD->getType();
            for (auto target : cur_decltypes) {
              if (type == target) {
                auto content = stringutils::rangetoStr(
                    *(Result.SourceManager), DL->getSourceRange());
                auto DL_type = stringutils::rangetoStr(
                    *(Result.SourceManager), target->getSourceRange());
                DL_type = DL_type.substr(8);
                DL_type.pop_back();
                auto DL_name = "mut_71_" + to_string(getrandom::getRandomIndex(1000));
                auto DL_template = "template<int " + DL_name + ">using " +
                                   DL_name + "=" + DL_type + ";\n";
                DL_template = "/*mut71*/" + DL_template;
                llvm::outs() << DL_template << '\n';
                string DL_decl = DL_name + "<0>";
                content.replace(content.find("decltype"), 8, DL_decl);
                Rewrite.ReplaceText(DL->getBeginLoc(), 0, DL_template);
                Rewrite.ReplaceText(
                    CharSourceRange::getTokenRange(DL->getSourceRange()),
                    content);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_71::MutatorASTConsumer_71::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto decltype_matcher = qualType(hasDeclaration(decltypeType())).bind("Decltypes");
    auto declstmt_matcher = declStmt().bind("Declstmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decltype_matcher, &callback);
    matchFinder.addMatcher(declstmt_matcher, &callback);
    matchFinder.matchAST(Context);
}