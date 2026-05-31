//header file
#pragma once
#include "Mutator_base.h"

/**
 * Break_Template_Instantiation_With_Typedef_Or_Using_257
 */ 
class MutatorFrontendAction_257 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(257)

private:
    class MutatorASTConsumer_257 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_257(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasDecl *> alias_decls;
    };
};

//source file
#include "../include/Break_Template_Instantiation_With_Typedef_Or_Using_257.h"

// ========================================================================================================
#define MUT257_OUTPUT 1

void MutatorFrontendAction_257::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXUnresolvedConstructExpr>("MultiType")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find('<') != string::npos &&
          content.rfind('>') != string::npos) {
        auto args = content.substr(content.find('<') + 1,
                                   content.rfind('>') - content.find('<') - 1);
        auto arglist = stringutils::split(args, ',');
        llvm::outs() << args << '\n';
        if (arglist.size() == 0)
          return;
        size_t index = getrandom::getRandomIndex(arglist.size() - 1);
        auto alias = arglist[index];
        if (alias.find("::") != string::npos)
          return;
        alias = stringutils::trim(alias);
        if (alias.find(' ') != string::npos)
          alias = alias.substr(alias.rfind(' ') + 1);
        llvm::outs() << alias << '\n';
        string def = "";
        int choice = getrandom::getRandomIndex(1);
        if (choice == 0)
          def = "typedef " + arglist[index] + " " + alias + ";\n";
        else
          def = "using " + alias + " = " + arglist[index] + ";\n";
        def = "/*mut257*/" + def;
        llvm::outs() << def << '\n';
        Rewrite.ReplaceText(MT->getBeginLoc(), 0, def);
      }
    } else if (auto *LD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>(
                   "AliasDecl")) {
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getBeginLoc()))
        return;
      alias_decls.push_back(LD);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "ClassDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          return;
        }
      }
      for (auto alias : alias_decls) {
        if (alias->getBeginLoc() < DL->getBeginLoc()) {
          auto ins = "using " + alias->getNameAsString() + ";\n";
          ins = "/*mut257*/" + ins;
          llvm::outs() << ins << '\n';
          Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
          return;
        }
      }
    }
}
  
void MutatorFrontendAction_257::MutatorASTConsumer_257::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto multi_type_matcher = cxxUnresolvedConstructExpr().bind("MultiType");
    auto alias_matcher = typeAliasDecl().bind("AliasDecl");
    auto class_matcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(multi_type_matcher, &callback);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}