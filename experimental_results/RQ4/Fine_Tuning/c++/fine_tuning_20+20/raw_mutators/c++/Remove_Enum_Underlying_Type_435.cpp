//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Enum_Underlying_Type_435
 */ 
class MutatorFrontendAction_435 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(435)

private:
    class MutatorASTConsumer_435 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_435(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypedefNameDecl *> cur_typedefs;
    };
};

//source file
#include "../include/remove_enum_underlying_type_435.h"

// ========================================================================================================
#define MUT435_OUTPUT 1

void MutatorFrontendAction_435::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->getIntegerTypeSourceInfo() == nullptr)
        return;
      auto underlying_type = DL->getIntegerType();
      auto underlying_type_str = underlying_type.getAsString();
      llvm::outs() << underlying_type_str << '\n';
      for (std::size_t j = 0; j < cur_typedefs.size(); ++j) {
        auto td = cur_typedefs[j];
        auto td_type = td->getUnderlyingType();
        auto td_type_str = td_type.getAsString();
        if (td_type_str == underlying_type_str) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 td->getSourceRange());
          llvm::outs() << content << '\n';
          if (content.find("typedef") != string::npos) {
            content.replace(content.find("typedef"), 7, "struct ");
            content.replace(content.rfind(';'), 1, ";\n");
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(td->getSourceRange()), content);
          }
        }
      }
    } else if (auto *TD =
                   Result.Nodes.getNodeAs<clang::TypedefNameDecl>("Typedefs")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      cur_typedefs.push_back(TD);
    }
}
  
void MutatorFrontendAction_435::MutatorASTConsumer_435::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = enumDecl().bind("Enums");
    auto typedef_matcher = typedefNameDecl().bind("Typedefs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(typedef_matcher, &callback);
    matchFinder.matchAST(Context);
}