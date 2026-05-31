//source file
#include "../include/misalign_pointer_to_member_163.h"

// ========================================================================================================
#define MUT163_OUTPUT 1

void MutatorFrontendAction_163::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
            return;

        //Record the node information to be used in the mutation process
        targetField = FD;
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
            return;

        if (!targetField)
            return;

        //Get the source code text of target node
        auto funcText = stringutils::rangetoStr(*(Result.SourceManager),
                                                FD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string fieldName = targetField->getNameAsString();
        std::string structName = targetField->getParent()->getNameAsString();
        std::string mutation = "anotherObj.*ptrToMember; /*mut163*/";

        size_t pos = funcText.find(fieldName);
        if (pos != std::string::npos) {
            funcText.replace(pos, fieldName.length(), mutation);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}

void MutatorFrontendAction_163::MutatorASTConsumer_163::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto fieldMatcher = fieldDecl(hasParent(cxxRecordDecl())).bind("fieldDecl");
    auto funcMatcher = functionDecl(hasBody(compoundStmt())).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}