//source file
#include "../include/Mutator_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *AT = VD->getType()->getAs<clang::TemplateSpecializationType>()) {
        if (AT->getTemplateName().getAsTemplateDecl()->getNameAsString() == "array") {
          const clang::TemplateArgument &arg = AT->getArg(0);
          if (arg.getKind() == clang::TemplateArgument::Type) {
            clang::QualType elemType = arg.getAsType();
            std::string elemTypeStr = elemType.getAsString();
            std::string varName = VD->getNameAsString();
            std::string initializer = "{ " + elemTypeStr + " instance; " + varName + " = {instance}; }";
            
            //Perform mutation on the source code text by applying string replacement
            Rewrite.ReplaceText(VD->getSourceRange(), initializer);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType(hasDeclaration(namedDecl(hasName("std::array")))))).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}