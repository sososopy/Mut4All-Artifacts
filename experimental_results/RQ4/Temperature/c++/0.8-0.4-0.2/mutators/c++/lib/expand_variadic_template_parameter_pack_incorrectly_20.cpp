//source file
#include "../include/expand_variadic_template_parameter_pack_incorrectly_20.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Stmt.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <string>

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<FunctionTemplateDecl>("FuncTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (FTD->getTemplatedDecl()->hasBody()) {
        auto body = FTD->getTemplatedDecl()->getBody();
        auto bodyText = Rewrite.getRewrittenText(body->getSourceRange());
        
        // Find the position to insert the incorrect expansion
        size_t insertPos = bodyText.find("{") + 1;
        
        std::string incorrectExpansion = R"(
          /*mut20*/ auto incorrectLambda = [args...]<typename T>(T t) {
            auto nestedLambda = []<typename...>(Args... expandedArgs) {};
          };
        )";
        
        bodyText.insert(insertPos, incorrectExpansion);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl())).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}