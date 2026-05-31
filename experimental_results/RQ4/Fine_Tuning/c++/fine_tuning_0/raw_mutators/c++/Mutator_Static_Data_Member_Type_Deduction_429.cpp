//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Static_Data_Member_Type_Deduction_429
 */ 
class MutatorFrontendAction_429 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(429)

private:
    class MutatorASTConsumer_429 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_429(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TypeAliasDecl *type_alias_decl;
        const clang::CXXRecordDecl *record_decl;
        const clang::VarDecl *var_decl;
    };
};

//source file
#include "../include/Mutator_Static_Data_Member_Type_Deduction_429.h"

// ========================================================================================================
#define MUT429_OUTPUT 1

void MutatorFrontendAction_429::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TAD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAliasDecl")) {
      if (!TAD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TAD->getLocation()))
        return;
      type_alias_decl = TAD;
      llvm::outs() << "type alias decl: "
                   << stringutils::rangetoStr(*(Result.SourceManager),
                                              TAD->getSourceRange())
                   << '\n';
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "CXXRecordDecl")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      if (!RD->isClass())
        return;
      record_decl = RD;
      llvm::outs() << "record decl: "
                   << stringutils::rangetoStr(*(Result.SourceManager),
                                              RD->getSourceRange())
                   << '\n';
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->isStaticDataMember())
        return;
      var_decl = VD;
      llvm::outs() << "var decl: "
                   << stringutils::rangetoStr(*(Result.SourceManager),
                                              VD->getSourceRange())
                   << '\n';
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isMain())
        return;
      llvm::outs() << "func decl: "
                   << stringutils::rangetoStr(*(Result.SourceManager),
                                              FD->getSourceRange())
                   << '\n';
      if (type_alias_decl == nullptr || record_decl == nullptr ||
          var_decl == nullptr)
        return;
      auto alias_name = type_alias_decl->getNameAsString();
      auto record_name = record_decl->getNameAsString();
      auto var_name = var_decl->getNameAsString();
      auto var_type = stringutils::rangetoStr(*(Result.SourceManager),
                                              var_decl->getTypeSourceInfo()
                                                  ->getTypeLoc()
                                                  .getSourceRange());
      auto var_value = stringutils::rangetoStr(
          *(Result.SourceManager), var_decl->getInit()->getSourceRange());
      auto record_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    record_decl->getSourceRange());
      auto alias_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   type_alias_decl->getSourceRange());
      auto func_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  FD->getSourceRange());
      llvm::outs() << "alias_name: " << alias_name << '\n';
      llvm::outs() << "record_name: " << record_name << '\n';
      llvm::outs() << "var_name: " << var_name << '\n';
      llvm::outs() << "var_type: " << var_type << '\n';
      llvm::outs() << "var_value: " << var_value << '\n';
      llvm::outs() << "record_content: " << record_content << '\n';
      llvm::outs() << "alias_content: " << alias_content << '\n';
      llvm::outs() << "func_content: " << func_content << '\n';
      auto new_var_decl = "static auto " + var_name + " = " + var_value;
      llvm::outs() << "new_var_decl: " << new_var_decl << '\n';
      auto new_record_content = record_content;
      new_record_content.replace(record_content.find(var_type),
                                 var_type.length(), "auto");
      llvm::outs() << "new_record_content: " << new_record_content << '\n';
      auto new_alias_content = alias_content;
      new_alias_content.replace(alias_content.find(record_name),
                                record_name.length(), new_record_content);
      llvm::outs() << "new_alias_content: " << new_alias_content << '\n';
      auto new_func_content = func_content;
      new_func_content.replace(func_content.find(record_name),
                               record_name.length(), new_alias_content);
      llvm::outs() << "new_func_content: " << new_func_content << '\n';
      new_func_content.replace(new_func_content.find(var_type),
                               var_type.length(), "auto");
      llvm::outs() << "new_func_content: " << new_func_content << '\n';
      new_func_content.replace(new_func_content.find(var_name),
                               var_name.length(), "mut429");
      llvm::outs() << "new_func_content: " << new_func_content << '\n';
      new_func_content.replace(new_func_content.find(var_value),
                               var_value.length(), "mut429");
      llvm::outs() << "new_func_content: " << new_func_content << '\n';
      Rewrite.ReplaceText(FD->getSourceRange(), new_func_content);
    }
}
  
void MutatorFrontendAction_429::MutatorASTConsumer_429::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto alias_matcher = typeAliasDecl().bind("TypeAliasDecl");
    auto record_matcher = cxxRecordDecl().bind("CXXRecordDecl");
    auto var_matcher = varDecl().bind("VarDecl");
    auto func_matcher = functionDecl().bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.addMatcher(record_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}