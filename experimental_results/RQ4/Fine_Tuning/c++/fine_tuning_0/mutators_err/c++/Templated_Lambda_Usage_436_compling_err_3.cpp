//header file
#pragma once
#include "Mutator_base.h"

/**
 * Templated_Lambda_Usage_436
 */ 
class MutatorFrontendAction_436 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(436)

private:
    class MutatorASTConsumer_436 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_436(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<std::string> type_names;
    };
};

//source file
#include "../include/Templated_Lambda_Usage_436.h"

// ========================================================================================================
#define MUT436_OUTPUT 1

void MutatorFrontendAction_436::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    //Get the source code text of target node
    auto name = DL->getNameAsString();
    //Perform mutation on the source code text by applying string replacement
    llvm::outs() << name << '\n';
    type_names.push_back(name);
  } else if (auto *TL = Result.Nodes.getNodeAs<clang::LambdaExpr>("TPLambda")) {
    if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TL->getBeginLoc()))
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), TL->getSourceRange());
    auto template_params = TL->getTemplateParameterList();
    for (auto param : *template_params) {
      llvm::outs() << param->getNameAsString() << '\n';
    }
    auto param = (*template_params)[0];
    auto param_name = param->getNameAsString();
    std::string replacement;
    if (type_names.size() == 0) {
      replacement = "int";
    } else {
      size_t index = getrandom::getRandomIndex(type_names.size() - 1);
      replacement = type_names[index];
    }
    //Perform mutation on the source code text by applying string replacement
    //Replace the original AST node with the mutated one
    llvm::outs() << replacement << '\n';
    llvm::outs() << content << '\n';
    stringutils::strReplace(content, param_name, replacement);
    llvm::outs() << content << '\n';
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TL->getSourceRange()), content);
  }
}
  
void MutatorFrontendAction_436::MutatorASTConsumer_436::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto tplambda_matcher = lambdaExpr(has(hasTemplateParameterList())).bind("TPLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(tplambda_matcher, &callback);
    matchFinder.matchAST(Context);
}