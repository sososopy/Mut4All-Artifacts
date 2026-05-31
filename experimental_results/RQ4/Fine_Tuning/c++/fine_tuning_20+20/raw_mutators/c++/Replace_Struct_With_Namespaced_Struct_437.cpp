//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_With_Namespaced_Struct_437
 */ 
class MutatorFrontendAction_437 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(437)

private:
    class MutatorASTConsumer_437 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_437(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> enum_names;
        std::vector<std::string> struct_names;
    };
};

//source file
#include "../include/replace_struct_with_namespaced_struct_437.h"

// ========================================================================================================
#define MUT437_OUTPUT 1

void MutatorFrontendAction_437::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      auto decls = ST->decls();
      std::string new_struct = "namespace ";
      new_struct += ST->getNameAsString();
      new_struct += "{\n";
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Enum) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 decl->getSourceRange());
          new_struct += content + "\n";
          enum_names.push_back(decl->getNameAsString());
        }
      }
      new_struct += "}\n";
      struct_names.push_back(ST->getNameAsString());
      llvm::outs() << new_struct;
      Rewrite.ReplaceText(ST->getSourceRange(), new_struct);
    } else if (auto *EN = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!EN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     EN->getLocation()))
        return;
      auto name = EN->getNameAsString();
      for (auto target : enum_names) {
        if (target == name)
          return;
      }
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             EN->getSourceRange());
      content = "/*mut437*/" + content;
      Rewrite.ReplaceText(EN->getSourceRange(), content);
    } else if (auto *SR = Result.Nodes.getNodeAs<clang::DeclRefExpr>(
                   "StructRef")) {
      if (!SR || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SR->getLocation()))
        return;
      auto name = SR->getNameInfo().getAsString();
      for (auto target : struct_names) {
        if (target == name) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 SR->getSourceRange());
          content = "/*mut437*/" + content;
          Rewrite.ReplaceText(SR->getSourceRange(), content);
        }
      }
    }
}

void MutatorFrontendAction_437::MutatorASTConsumer_437::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto enum_matcher = enumDecl().bind("Enums");
    auto structref_matcher = declRefExpr().bind("StructRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(structref_matcher, &callback);
    matchFinder.matchAST(Context);
}