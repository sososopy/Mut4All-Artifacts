//source file
#include "../include/specify_template_return_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getReturnType()->isUndeducedType()) {
            //Get the source code text of target node
            auto returnType = FD->getReturnType().getAsString();
            if (returnType == "auto") {
                //Perform mutation on the source code text by applying string replacement
                returnType = "int"; // Specify a return type
                Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), returnType);
                if (!FD->doesThisDeclarationHaveABody()) {
                    auto bodyRange = FD->getBody()->getSourceRange();
                    std::string bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
                    // Insert return statement if missing
                    if (bodyText.find("return") == std::string::npos) {
                        bodyText.insert(bodyText.find_last_of('}'), " return 0;");
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
                    }
                }
            }
        }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VariableDecl")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (VD->getType().getAsString() == "decltype(auto)") {
            //Perform mutation on the source code text by applying string replacement
            Rewrite.ReplaceText(VD->getTypeSourceInfo()->getTypeLoc().getSourceRange(), "int");
        }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = functionDecl(isTemplateInstantiation(), returns(autoType())).bind("FunctionTemplate");
    DeclarationMatcher decltypeMatcher = varDecl(hasType(autoType())).bind("VariableDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(decltypeMatcher, &callback);
    matchFinder.matchAST(Context);
}